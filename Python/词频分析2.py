def gettext():
    txt = open("D:\toana.txt","r",errors='ignore').read()
    txt = txt.lower()
    for ch in '!"#$&()*+,-./:;<=>?@[\\]^_{|}·~‘’':
        txt = txt.replace(ch,"")
    return txt

txt = gettext()
words = txt.split()
counts = {}
for word in words:
    counts[word] = counts.get(word,0) + 1


items = list(counts.items())
items.sort(key=lambda x:x[1],reverse=True)
for i in range(500):
    word,count = items[i]
    print("{0:<500}{1:>5}".format(word,count))
