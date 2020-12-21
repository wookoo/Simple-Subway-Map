
import json
import os
for i in os.listdir():
    if(not i.endswith(".json")):
        continue
    print(i)

assert False
for i in os.listdir():

    if (not i.endswith(".json") and i == "final.json"):
        continue

    with open('final.json',encoding="utf8") as f:
        data1 = json.load(f)

    with open(i,encoding="utf8") as f:
        print(i)
        data2 = json.load(f)
    newJson = dict(data1,**data2)
    for i in data1.keys():
        if(i in data2):
            newJson[i] = (dict(data1[i],**data2[i]))

    with open("final.json","w",encoding="utf8") as f:
        f.write(json.dumps(newJson,ensure_ascii=False))
