
ftele1 = open('D:\Onedrive\资料\Desktop\文本1.txt', 'r',encoding='UTF-8')
ftele2 = open('D:\Onedrive\资料\Desktop\文本2.txt', 'r',encoding='UTF-8')

ftele1.readline()
ftele2.readline()#跳过第一行
lines1 = ftele1.readlines()
lines2 = ftele2.readlines()

dic1 = {}
dic2 = {}
lines = []

for line in lines1:
    elements = line.split()
    dic1[elements[0]] = str(elements[1])

for line in lines2:
    elements = line.split()
    dic2[elements[0]] = str(elements[1])



for key in dic1:
    s = ''
    if key in dic2.keys():
        s='\t'.join([str(key),dic1[key],dic2[key]])
        s += '\n'
    else:
        s = '\t'.join([str(key),dic1[key],str('------')])
        s += '\n'
    lines.append(s)

for key in dic2:
    s = ''
    if key not in dic1.keys():
        s = '\t'.join([str(key),str('------'),dic2[key]])
        s += '\n'
    lines.append(s)

ftele3 = open('D:\Onedrive\资料\Desktop\Mix.txt','w')
ftele3.writelines(lines)

ftele3.close()
ftele1.close()
ftele2.close()

print("The addressBooks are merged")
                             
