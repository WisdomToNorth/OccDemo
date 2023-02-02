# -*- coding:utf-8 -*- 
# 把kindle里面的摘录，按书名分开保存

import re
from collections import deque
import os

#kindle的摘录通常格式如下，每两行====== 之间的内容，固定4行，分别是书名，摘录的位置和时间，空行，摘录或笔记内容
'''
==========
盐铁往事：两千年前的货币战争 (史上十大口水战) (还是定风波)
- 您在位置 #184-186的标注 | 添加于 2018年4月9日星期一 下午8:10:32

专家最擅长的事是把用常识就能判断的事，用一系列专业名词和专业仪器来
证明你的判断是错误的。比如一个大楼你说有问题，墙上都裂了大缝了，
但专家能拿一个你从没听过的理论和一个你从没见过的仪器证明就是
没有问题，墙上的大缝只是天冷了大楼打了个喷嚏，过一会子就好了。
==========  
'''

def fetch_note(file_path):
    content = deque(maxlen=6)
    with open(file_path,encoding='utf-8') as fp:        
        for line in fp:
            content.append(line.strip())
            #如果content里面有超过两行，并且第一个元素是=====，当前行也是=====，说明匹配到了一段完整的摘录
            if len(content) >= 2 and re.match(r'^=+',line) and re.match(r'^=+',content[0]):

                yield content


def write_files(in_file_path,out_file_dir = None): #要读取的文件路径，以及输出文件的目录
    out_dir = out_file_dir if out_file_dir else os.path.dirname(in_file_path) #如果没提供目标目录，则把读取文件的路径作为输出路径

    if os.path.isfile(out_dir):
        print('目标目录是个文件，请确认!')
        return False
    
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
        
    book_names = {}  #保存文件的引用对象，格式为{文件名:文件打开的对象} 即: {file_name : open(file_name,'w') }
    for x in fetch_note(in_file_path):
        book_name,note_time,note_content = x[1],re.sub('^.*?添加于\s+','',x[2]),x[4] #书名、摘录时间、摘录或笔记内容
        #print(book_name,note_time,note_content)
        if book_name not in book_names:
            boo_name_full_path = os.path.join(out_dir,book_name) + '.txt' 
            book_name_fp = open(boo_name_full_path,'a',encoding='utf-8') # 用追加模式
            book_names[book_name] = book_name_fp
            
        book_names[book_name].write(note_time + '\n')
        book_names[book_name].write(note_content + '\n\n')
        
    for bk_fp in book_names.values():
        bk_fp.close()
        
    
if __name__ == '__main__':
    file_path = 'D:/My Clippings.txt'
    write_files(file_path)

