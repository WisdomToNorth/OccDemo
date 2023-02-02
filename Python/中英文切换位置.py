import os
import re
file_dir =r"H:\\temp"

for file in os.listdir(file_dir):

    namestring = os.path.basename(file).split('.')[0]
    subfix=os.path.basename(file).split('.')[1]
    print(namestring)
    str_ch = re.sub("[A-Za-z0-9\,\。\_-]", "", namestring)
    str_en = re.sub("[\u4e00-\u9fa5\,\。_]", "", namestring).upper()
    newname = str_en+"_"+str_ch+'.'+subfix
    print(newname+'\n')
    os.rename(os.path.join(file_dir,file),os.path.join(file_dir,newname))
    #os.rename(file,newname)

# import re
# str = "重123出江湖hello的地H方。。的,world"
# str = re.sub("[A-Za-z\u4e00-\u9fa5\,\。]", "", str)
# print(str)