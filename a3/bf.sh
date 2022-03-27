# run prediction script on k=5,10,15 and save results in a file
# $1 is n $2 is c
# ./HNSWpred.sh final/ 5 anz_ts/user.txt pred_2_5_5.txt >>
./HNSWpred.sh final/ 5 anz_ts/user.txt pred_$1_$2_5.txt > time_$1_$2_5.txt
./HNSWpred.sh final/ 10 anz_ts/user.txt pred_$1_$2_10.txt > time_$1_$2_10.txt
./HNSWpred.sh final/ 15 anz_ts/user.txt pred_$1_$2_15.txt > time_$1_$2_15.txt