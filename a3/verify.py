# compare groundtruth and output

'''
Precision@k = {items in top K prediction} ⋂ {Items in ground truth} / k
'''
def precision(topk, gt,k):
    # topk and gt are 2d list
    # compute intersection for each row
    prec=0
    for i in range(len(topk)):
        topk[i] = set(topk[i])
        gt[i] = set(gt[i])
        prec += len(topk[i].intersection(gt[i]))/len(topk[i])
    return prec/len(topk)

'''
Recall @k = {items in top K prediction} ⋂ {Items in ground truth} / total items in ground truth
'''

def recall(topk, gt,k):
    rec=0
    for i in range(len(topk)):
        topk[i] = set(topk[i])
        gt[i] = set(gt[i])
        rec += len(topk[i].intersection(gt[i]))/len(topk[i])
    return rec/len(topk)

def cal():
    file1=open('output.txt','r')
    file2=open('groundtruth.txt','r')
    topk=[]
    gt=[]
    for line in file1:
        line = line.strip().split(' ')
        topk.append(line)
    for line in file2:
        line = line.strip().split(' ')
        gt.append(line)
    # print(precision(topk,gt,10))
    # print(recall(topk,gt,10))

from code import interact
import sys
pred = []
gh = []
f1="tmp3_or.txt"
f2="anz_ts/user_gt.txt"
if(len(sys.argv)>2):
    f1=sys.argv[1]
    f2=sys.argv[2]
with open(f1) as fs:
    pred = fs.readlines()
with open(f2) as fs:
    gh = fs.readlines()

def makeints(pred):
    k = []
    for p in pred:
        l = str.split(p)
        # print(l)
        ks = []
        for v in l:
            ks.append(int(v))
        k.append(ks)
    return k

pred = makeints(pred)
gh = makeints(gh)

precision = 0
recall = 0

assert(len(pred) == len(gh))
test=0
for i in range(len(pred)):
    inter =0
    for p in pred[i]:
        if p in gh[i]:
            inter+=1
            # print("common :", p)
    test+=inter
    if(len(pred[i]) > 0):
        precision += inter/len(pred[i])
    if(len(gh[i]) > 0):
        recall += inter/len(gh[i])

print("precision ", precision * 100 /len(pred))
print("recall ", recall * 100/len(gh))
    
print(test)