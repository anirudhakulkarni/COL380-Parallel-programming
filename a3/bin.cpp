#include "includes.hpp"

template <typename Arg, typename... Args>
void debug(int rank, Arg &&arg, Args &&...args)
{
    // if (rank != 2) return;   
    bool verbose = 0;
    if (!verbose) return;
    cout << "RANK: " << rank << " || ";
    cout << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(cout << " || " << std::forward<Args>(args)), 0)...};
    cout << endl;
}

int parprocess_int(MPI_File in, FILE wfp, const int rank, const int size, const int overlap)
{
    MPI_Offset globalstart;
    int mysize;

    /* read in relevant chunk of file into "chunk",
     * which starts at location in the file globalstart
     * and has size mysize
     */

    MPI_Offset globalend;
    MPI_Offset filesize;

    /* figure out who reads what */
    MPI_File_get_size(*in, &filesize);
    filesize--; /* get rid of text file eof */
    mysize = filesize / size;
    globalstart = rank * mysize;
    globalend = globalstart + mysize - 1; 
    if (rank == size - 1)
        globalend = filesize - 1;

    /* add overlap to the end of everyone's chunk except last proc... */
    if (rank != size - 1)
        globalend += overlap;

    mysize = globalend - globalstart + 1;
    vector<char> chunk(mysize);

    /* everyone reads in their part */
    MPI_File_read_at_all(*in, globalstart, &chunk.front(), mysize, MPI_CHAR, MPI_STATUS_IGNORE);
    // chunk[mysize] = '\0';
    // }

    /*
     * everyone calculate what their start and end really are by going
     * from the first newline after start to the first newline after the
     * overlap region starts (eg, after end - overlap + 1)
     */
    // cout<<chunk<<endl;
    int locstart = 0, locend = mysize - 1;
    if (rank != 0)
    {
        while (chunk[locstart] != '\n' && chunk[locstart] != ' ')
            locstart++;
        locstart++;
    }
    if (rank != size - 1)
    {
        locend -= overlap;
        do
        {
            locend++;
        } while (chunk[locend] != '\n' && chunk[locend] != ' ');
    }
    mysize = locend - locstart;

    /* "Process" our chunk by replacing non-space characters with '1' for
     * rank 1, '2' for rank 2, etc...
     */

    vector<int> values;
    vector<char> temp;
    debug (rank, chunk);
    debug(rank,locstart,locend, chunk.size());
    for (int i = locstart; i <= locend; i++)
    {
        char x = chunk[i];
        if (x != ' ' && x != '\n')
        {
                temp.push_back(x);
        }
        else if(temp.size()>0)
        {
                string str(temp.begin(), temp.end());
                // cout<<str<<endl;
                values.push_back(stoi(str));
                temp.clear();            
        }
    }
    if (temp.size())
    {
        string str(temp.begin(), temp.end());
        cout<<str<<endl;
        if(str.compare("-")!=0)
        values.push_back(stoi(str));
        temp.clear();
    }

    vector<int> numFloats(size);
    numFloats[rank] = values.size();
    vector<int> recvCounts(size, 1);
    vector<int> numFloatsStarts(size);
    for (int i = 0; i < numFloats.size(); i++)
        numFloatsStarts[i] = i;
    cout<<values<<endl;
    MPI_Allgatherv(MPI_IN_PLACE, recvCounts[rank], MPI_INT,
                   &numFloats.front(), &recvCounts.front(), &numFloatsStarts.front(), MPI_INT, MPI_COMM_WORLD);

    for (int i = 1; i < numFloats.size(); i++)
    {
        numFloats[i] += numFloats[i - 1];
    }
    int ans = numFloats[size-1];
    numFloats.insert(numFloats.begin(), 0);
    numFloats.pop_back();
    // cout<<
    debug (rank, values);

    // FILE *wfp = fopen(out.c_str(), "wb");
    fseek(wfp, numFloats[rank] * 4, SEEK_SET);
    fwrite(&values.front(), values.size(), sizeof(int), wfp);
    fclose(wfp);

    /* output the processed file */

    // MPI_File_write_at_all(*out, (MPI_Offset)(globalstart+(MPI_Offset)locstart), &(chunk[locstart]), mysize, MPI_CHAR, MPI_STATUS_IGNORE);

    return ans;
}

int parprocess_float(MPI_File in, FILE wfp, const int rank, const int size, const int overlap)
{
    MPI_Offset globalstart;
    int mysize;

    /* read in relevant chunk of file into "chunk",
     * which starts at location in the file globalstart
     * and has size mysize
     */
    // {
    MPI_Offset globalend;
    MPI_Offset filesize;

    /* figure out who reads what */
    MPI_File_get_size(*in, &filesize);
    filesize--; /* get rid of text file eof */
    mysize = filesize / size;
    globalstart = rank * mysize;
    globalend = globalstart + mysize - 1; 
    if (rank == size - 1)
        globalend = filesize - 1;

    /* add overlap to the end of everyone's chunk except last proc... */
    if (rank != size - 1)
        globalend += overlap;

    mysize = globalend - globalstart + 1;

    vector<char> chunk(mysize);

    /* everyone reads in their part */
    MPI_File_read_at_all(*in, globalstart, &chunk.front(), mysize, MPI_CHAR, MPI_STATUS_IGNORE);
   

    /*
     * everyone calculate what their start and end really are by going
     * from the first newline after start to the first newline after the
     * overlap region starts (eg, after end - overlap + 1)
     */

    int locstart = 0, locend = mysize - 1;
    if (rank != 0)
    {
        while (chunk[locstart] != '\n' && chunk[locstart] != ' ')
            locstart++;
        locstart++;
    }
    if (rank != size - 1)
    {
        locend -= overlap;
        do
        {
            locend++;
        } while (chunk[locend] != '\n' && chunk[locend] != ' ');
    }
    mysize = locend - locstart;

    /* "Process" our chunk by replacing non-space characters with '1' for
     * rank 1, '2' for rank 2, etc...
     */

    vector<float> values;
    vector<char> temp;
    debug(rank,locstart,locend, chunk.size());
    for (int i = locstart; i <= locend; i++)
    {
        char x = chunk[i];
        if (x != ' ' && x != '\n')
        {
                temp.push_back(x);
        }
        else if(temp.size()>0)
        {
                string str(temp.begin(), temp.end());
                values.push_back(stof(str));
                temp.clear();
            
        }
    }
    if (temp.size())
    {
        string str(temp.begin(), temp.end());
        if(str.compare("-")!=0)
        values.push_back(stof(str));
        temp.clear();
    }

    vector<int> numFloats(size);
    numFloats[rank] = values.size();
    vector<int> recvCounts(size, 1);
    vector<int> numFloatsStarts(size);
    for (int i = 0; i < numFloats.size(); i++)
        numFloatsStarts[i] = i;

    MPI_Allgatherv(MPI_IN_PLACE, recvCounts[rank], MPI_INT,
                   &numFloats.front(), &recvCounts.front(), &numFloatsStarts.front(), MPI_INT, MPI_COMM_WORLD);

    for (int i = 1; i < numFloats.size(); i++)
    {
        numFloats[i] += numFloats[i - 1];
    }
    int ans = numFloats[size-1];
    numFloats.insert(numFloats.begin(), 0);
    numFloats.pop_back();
    // cout<<values<<endl;
    debug (rank, values);

    // FILE *wfp = fopen(out.c_str(), "wb");
    fseek(wfp, numFloats[rank] * 4, SEEK_SET);
    if(values.size()!=numFloats[rank+1]-numFloats[rank]){
        cout << "ERROR " << values.size()<<" || "<< numFloats[rank + 1] << " || " << numFloats[rank]  << endl;
    }
    // fwrite(&values.front(), sizeof(float), values.size(), wfp);
    fwrite(&values.front(), values.size(), sizeof(float), wfp);
    fclose(wfp);
    return ans;
}

int main(int argc, char **argv)
{

    MPI_File vect_in, user_in, index_in;
    int rank, size;
    int ierr;
    const int overlap = 12;

    string in = argv[1];  // input_folder
    string out = argv[2]; // output folder

    string vect_file_name = in + "/" + "vect.txt";
    string user_file_name = in + "/" + "user.txt";
    string index_file_name = in + "/" + "index.txt";

    string vect_bin_name = out + "/" + "vect.bin";
    string user_bin_name = out + "/" + "user.bin";
    string index_bin_name = out + "/" + "index.bin";
    
    FILE *wfp_index = fopen(index_bin_name.c_str(), "wb");
    FILE *wfp_user = fopen(user_bin_name.c_str(), "wb");
    FILE *wfp_vect = fopen(vect_bin_name.c_str(), "wb");

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    

    // debug (rank, "r1");
    ierr = MPI_File_open(MPI_COMM_WORLD, vect_file_name.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &vect_in);
    ierr = MPI_File_open(MPI_COMM_WORLD, user_file_name.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &user_in);
    ierr = MPI_File_open(MPI_COMM_WORLD, index_file_name.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &index_in);

    // int index_len=25;
    int index_len = parprocess_int(&index_in, wfp_index, rank, size, overlap);
    // debug(rank, "r2.2");
    int num_users = parprocess_float(&user_in, wfp_user, rank, size, overlap);
    // debug(rank, "r2.4");
    int num_news = parprocess_float(&vect_in, wfp_vect, rank, size, overlap);
    // debug(rank, "r2.6");
    MPI_File_close(&vect_in);
    MPI_File_close(&index_in);
    MPI_File_close(&user_in);
    // debug(rank, "r3");
    // MPI_File_close(&out);

    write_header_file(in, out, num_users, num_news, index_len);

    // debug(rank, "r4");
    MPI_Finalize();
    return 0;
}