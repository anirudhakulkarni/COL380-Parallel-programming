# calculate cosine similarity between two sequences
from psutil import swap_memory


def cosine_similarity(seq1, seq2):
    """
    Calculate the cosine similarity between two sequences.
    """
    if len(seq1) != len(seq2):
        raise ValueError("Sequences must be of equal length.")
    if len(seq1) == 0:
        raise ValueError("Sequences must not be empty.")
    # calculate the dot product
    dot_product = 0
    for i in range(len(seq1)):
        dot_product += seq1[i] * seq2[i]
    # calculate the magnitude of the first sequence
    seq1_magnitude = 0
    for i in range(len(seq1)):
        seq1_magnitude += seq1[i]**2
    seq1_magnitude = seq1_magnitude**0.5
    # calculate the magnitude of the second sequence
    seq2_magnitude = 0
    for i in range(len(seq2)):
        seq2_magnitude += seq2[i]**2
    seq2_magnitude = seq2_magnitude**0.5
    if seq1_magnitude == 0 or seq2_magnitude == 0:
        return 0
    return dot_product / (seq1_magnitude * seq2_magnitude)

def cosine_dist_on_list(list1,list2):
    i=0
    j=0
    for seq1 in list1:
        for seq2 in list2:
            f=cosine_similarity(seq1,seq2)
            print(i,j,end=" ")
            print(1-f)
            j+=1
        i+=1
        j=0
        print("---------------------")
# take input from file
with open('anz_dd/vect.txt') as f:
    # read 2d list
    list1 = [line.split() for line in f]
    # convert 2d list to 2d list of float
    seqs = [[float(i) for i in seq] for seq in list1]
with open('user.txt') as f:
    list1 = [line.split() for line in f]
    seqs2 = [[float(i) for i in seq] for seq in list1]
# print(seqs)
# print(seqs2)
cosine_dist_on_list(seqs,seqs2)
