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
    print(precision(topk,gt,10))
    print(recall(topk,gt,10))

cal()