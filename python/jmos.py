import json
from tkinter import *

window = Tk()

window.title("JMOS")

frame1 = Frame(window)
frame1.pack(fill = X)

titleLbl = Label(frame1, text = "JMOS temporary basic front-end")
titleLbl.pack(fill = X)

a = dict()
a["mod"] = "Mod name:"
a["game"] = "Game name:"
a["ID"] = "Nexusmods ID:"
a["main image"] = "Main image full URL:"
a["author"] = "Author name:"
a["categories"] = "Category:"
a["images"] = "Nexusmods image:"
a["description"] = "Description:"

lbl = dict()
ent = dict()
frame = dict()

i = 1
for key, val in a.items():
    frame[key] = Frame(window)
    frame[key].pack(fill = X)
    lbl[key] = Label(frame[key], text = val)
    lbl[key].pack(side = LEFT)
    ent[key] = Entry(frame[key])
    ent[key].pack(side = RIGHT)
    i = i + 1
#end

ifile = "../db.json"

j = open(ifile)
data = json.load(j)
j.close()

frame2 = Frame(window)
frame2.pack(fill = X)

def categoryAdd():
    pass
#end

categoryAddBtn = Button(frame2, text = "Add category", command = categoryAdd)
categoryAddBtn.pack(side = RIGHT)

def submit():
    data["Mods"][ent["mod"].get()] = dict()

    for val in [ "main image", "author", "description"]:
        if ent[val].get():
            data["Mods"][ent["mod"].get()][val] = ent[val].get()
        #end
    #end

    for val in [ "categories", "images" ]:
        if ent[val].get():
            data["Mods"][ent["mod"].get()][val] = [ ent[val].get() ]
        #end
    #end

    if ent["game"].get() and ent["ID"].get():
        data["Mods"][ent["mod"].get()]["id"] = {
            ent["game"].get() : int(ent["ID"].get())
        }
    #end
#end

submitBtn = Button(frame2, text = "Submit", command = submit)
submitBtn.pack(side = RIGHT)

window.mainloop()

print(json.dumps(data, indent = 2))
