
ftele1 = open('D:\Onedrive\资料\Desktop\文本1.txt', 'r',encoding='UTF-8')
ftele2 = open('D:\Onedrive\资料\Desktop\文本2.txt', 'r',encoding='UTF-8')

ftele1.readline()
ftele2.readline()#跳过第一行
lines1 = ftele1.readlines()
lines2 = ftele2.readlines()

list1_name = []
list1_tele = []
list2_name = []
list2_email = []
lines=[]

for line in lines1:
    elements = line.split(' ')
    list1_name.append(str(elements[0]))
    list1_tele.append(str(elements[1]))

for line in lines2:
    elements = line.split(' ')
    list2_name.append(str(elements[0]))
    list2_email.append(str(elements[1]))


for i in range(len(list1_name)):
    s = ''
    if list1_name[i] in list2_name:
        j = list2_name.index(list1_name[i])
        s = '\t'.join([list1_name[i],list1_tele[i],list2_email[j]])
        s += '\n'
        lines.append(s)
    else:
        s = '\t'.join([list1_name[i],list1_tele[i],str('-----')])
        s += '\n'
        lines.append(s)
                       

for i in range(len(list2_name)):
    s = ''
    if list2_name[i] not in list1_name:
        s = '\t'.join([list2_name[i],str('-----'),list2_email[i]])
        s += '\n'
        lines.append(s)

ftele3 = open('D:\Onedrive\资料\Desktop\Mix2.txt','w')
ftele3.writelines(lines)

ftele3.close()
ftele1.close()
ftele2.close()

print("The addressBooks are merged")
                             
