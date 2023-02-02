import re

def processLine(line, wordCounts):
    #用空格替换标点符号
    line=strip_symbol(line)
    #从每一行获取每个词
    words = line.split()
    for word in words:
        if len(word)>3:
            if word in wordCounts:
                wordCounts[word] += 1
            else:
                wordCounts[word] = 1

def strip_symbol(ustr):
#删除unicode字符串中英文的标点符号:param ustr:unicode字符串

    return re.sub('[’!"#$%&\'()*+,-./:;1234567890<=>?@[\\]^_`{|}~]+',' ',ustr)

#用户输入文件路径
#filename = input("enter a filename:").strip()
infile = open("D:\分析8.txt","r",encoding='UTF-8')

#建立用于统计词频的空字典
wordCounts = {}
for line in infile:
    
    processLine(line.lower(), wordCounts)
    
items = list(wordCounts.items())
key = list(wordCounts.keys())
items.sort(key=lambda x:x[1],reverse=True) #计数排序

##for i in range(10):
##    word,count = items[i]
##    print("{0:<10}{1:>5}".format(word,count)) #引号内为打印格式

f=open('D:\Onedrive\资料\Desktop\Analyse.txt',"w")
for i in range(7000):
    word,count = items[i]
    mix ='\t'.join([str(word),str(count)])
    f.write(mix) #引号内为输出格式
    f.write('\n')
f.close()
