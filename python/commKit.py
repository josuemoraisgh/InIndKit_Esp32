# https://www.devmedia.com.br/tkinter-interfaces-graficas-em-python/33956
import sqlite3
from tkinter import *

class Banco():
    def __init__(self):
        self.conexao = sqlite3.connect('banco.db')
        self.createTable()

    def createTable(self):
        c = self.conexao.cursor()

        c.execute("""create table if not exists usuarios (
                    idusuario integer primary key autoincrement ,
                    nome text,
                    telefone text,
                    email text,
                    usuario text,
                    senha text)""")
        self.conexao.commit()
        c.close()

class Usuarios(object):
    def __init__(self, idusuario = 0, nome = "", telefone = "",
        email = "", usuario = "", senha = ""):
        self.info = {}
        self.idusuario = idusuario
        self.nome = nome
        self.telefone = telefone
        self.email = email
        self.usuario = usuario
        self.senha = senha


    def insertUser(self):
        banco = Banco()
        try:

            c = banco.conexao.cursor()

            c.execute("insert into usuarios (nome, telefone, email, usuario, senha) values ('" + self.nome + "', '" + self.telefone + "', '" + self.email + "', '" + self.usuario + "', '" + self.senha + "' )")
            banco.conexao.commit()
            c.close()

            return "Usuário cadastrado com sucesso!"
        except:
            return "Ocorreu um erro na inserção do usuário"

    def updateUser(self):
        banco = Banco()
        try:

            c = banco.conexao.cursor()

            c.execute("update usuarios set nome = '" + self.nome + "', telefone = '" + self.telefone + "', email = '" + self.email + "', usuario = '" + self.usuario + "', senha = '" + self.senha + "' where idusuario = " + self.idusuario + " ")
            banco.conexao.commit()
            c.close()

            return "Usuário atualizado com sucesso!"
        except:
            return "Ocorreu um erro na alteração do usuário"

    def deleteUser(self):
        banco = Banco()
        try:

            c = banco.conexao.cursor()

            c.execute("delete from usuarios where idusuario = " + self.idusuario + " ")

            banco.conexao.commit()
            c.close()

            return "Usuário excluído com sucesso!"
        except:
            return "Ocorreu um erro na exclusão do usuário"

    def selectUser(self, idusuario):
        banco = Banco()
        try:

            c = banco.conexao.cursor()

            c.execute("select * from usuarios where idusuario = " + idusuario + "  ")

            for linha in c:
                self.idusuario = linha[0]
                self.nome = linha[1]
                self.telefone = linha[2]
                self.email = linha[3]
                self.usuario = linha[4]
                self.senha = linha[5]

            c.close()

            return "Busca feita com sucesso!"
        except:
            return "Ocorreu um erro na busca do usuário"

class Application:
    def __init__(self, master=None):
        self.fonte = ("Verdana", "8")

        self.container1 = Frame(master)
        self.container1["pady"] = 10
        self.container1.pack()
        self.container2 = Frame(master)
        self.container2["padx"] = 20
        self.container2["pady"] = 5
        self.container2.pack()
        self.container3 = Frame(master)
        self.container3["padx"] = 20
        self.container3["pady"] = 5
        self.container3.pack()
        self.container4 = Frame(master)
        self.container4["padx"] = 20
        self.container4["pady"] = 5
        self.container4.pack()
        self.container5 = Frame(master)
        self.container5["padx"] = 20
        self.container5["pady"] = 5
        self.container5.pack()
        self.container6 = Frame(master)
        self.container6["padx"] = 20
        self.container6["pady"] = 5
        self.container6.pack()
        self.container7 = Frame(master)
        self.container7["padx"] = 20
        self.container7["pady"] = 5
        self.container7.pack()
        self.container8 = Frame(master)
        self.container8["padx"] = 20
        self.container8["pady"] = 10
        self.container8.pack()
        self.container9 = Frame(master)
        self.container9["pady"] = 15
        self.container9.pack()

        self.titulo = Label(self.container1, text="Informe os dados :")
        self.titulo["font"] = ("Calibri", "9", "bold")
        self.titulo.pack ()

        self.lblidusuario = Label(self.container2,
        text="idUsuario:", font=self.fonte, width=10)
        self.lblidusuario.pack(side=LEFT)

        self.txtidusuario = Entry(self.container2)
        self.txtidusuario["width"] = 10
        self.txtidusuario["font"] = self.fonte
        self.txtidusuario.pack(side=LEFT)

        self.btnBuscar = Button(self.container2, text="Buscar",
        font=self.fonte, width=10)
        self.btnBuscar["command"] = self.buscarUsuario
        self.btnBuscar.pack(side=RIGHT)

        self.lblnome = Label(self.container3, text="Nome:",
        font=self.fonte, width=10)
        self.lblnome.pack(side=LEFT)

        self.txtnome = Entry(self.container3)
        self.txtnome["width"] = 25
        self.txtnome["font"] = self.fonte
        self.txtnome.pack(side=LEFT)

        self.lbltelefone = Label(self.container4, text="Telefone:",
        font=self.fonte, width=10)
        self.lbltelefone.pack(side=LEFT)

        self.txttelefone = Entry(self.container4)
        self.txttelefone["width"] = 25
        self.txttelefone["font"] = self.fonte
        self.txttelefone.pack(side=LEFT)

        self.lblemail= Label(self.container5, text="E-mail:",
        font=self.fonte, width=10)
        self.lblemail.pack(side=LEFT)

        self.txtemail = Entry(self.container5)
        self.txtemail["width"] = 25
        self.txtemail["font"] = self.fonte
        self.txtemail.pack(side=LEFT)

        self.lblusuario= Label(self.container6, text="Usuário:",
        font=self.fonte, width=10)
        self.lblusuario.pack(side=LEFT)

        self.txtusuario = Entry(self.container6)
        self.txtusuario["width"] = 25
        self.txtusuario["font"] = self.fonte
        self.txtusuario.pack(side=LEFT)

        self.lblsenha= Label(self.container7, text="Senha:",
        font=self.fonte, width=10)
        self.lblsenha.pack(side=LEFT)

        self.txtsenha = Entry(self.container7)
        self.txtsenha["width"] = 25
        self.txtsenha["show"] = "*"
        self.txtsenha["font"] = self.fonte
        self.txtsenha.pack(side=LEFT)

        self.bntInsert = Button(self.container8, text="Inserir",
        font=self.fonte, width=12)
        self.bntInsert["command"] = self.inserirUsuario
        self.bntInsert.pack (side=LEFT)

        self.bntAlterar = Button(self.container8, text="Alterar",
        font=self.fonte, width=12)
        self.bntAlterar["command"] = self.alterarUsuario
        self.bntAlterar.pack (side=LEFT)

        self.bntExcluir = Button(self.container8, text="Excluir",
        font=self.fonte, width=12)
        self.bntExcluir["command"] = self.excluirUsuario
        self.bntExcluir.pack(side=LEFT)

        self.lblmsg = Label(self.container9, text="")
        self.lblmsg["font"] = ("Verdana", "9", "italic")
        self.lblmsg.pack()


    def inserirUsuario(self):
        user = Usuarios()

        user.nome = self.txtnome.get()
        user.telefone = self.txttelefone.get()
        user.email = self.txtemail.get()
        user.usuario = self.txtusuario.get()
        user.senha = self.txtsenha.get()

        self.lblmsg["text"] = user.insertUser()

        self.txtidusuario.delete(0, END)
        self.txtnome.delete(0, END)
        self.txttelefone.delete(0, END)
        self.txtemail.delete(0, END)
        self.txtusuario.delete(0, END)
        self.txtsenha.delete(0, END)



    def alterarUsuario(self):
        user = Usuarios()

        user.idusuario = self.txtidusuario.get()
        user.nome = self.txtnome.get()
        user.telefone = self.txttelefone.get()
        user.email = self.txtemail.get()
        user.usuario = self.txtusuario.get()
        user.senha = self.txtsenha.get()

        self.lblmsg["text"] = user.updateUser()

        self.txtidusuario.delete(0, END)
        self.txtnome.delete(0, END)
        self.txttelefone.delete(0, END)
        self.txtemail.delete(0, END)
        self.txtusuario.delete(0, END)
        self.txtsenha.delete(0, END)



    def excluirUsuario(self):
        user = Usuarios()

        user.idusuario = self.txtidusuario.get()

        self.lblmsg["text"] = user.deleteUser()

        self.txtidusuario.delete(0, END)
        self.txtnome.delete(0, END)
        self.txttelefone.delete(0, END)
        self.txtemail.delete(0, END)
        self.txtusuario.delete(0, END)
        self.txtsenha.delete(0, END)


    def buscarUsuario(self):
        user = Usuarios()

        idusuario = self.txtidusuario.get()

        self.lblmsg["text"] = user.selectUser(idusuario)

        self.txtidusuario.delete(0, END)
        self.txtidusuario.insert(INSERT, user.idusuario)

        self.txtnome.delete(0, END)
        self.txtnome.insert(INSERT, user.nome)

        self.txttelefone.delete(0, END)
        self.txttelefone.insert(INSERT,user.telefone)

        self.txtemail.delete(0, END)
        self.txtemail.insert(INSERT, user.email)

        self.txtusuario.delete(0, END)
        self.txtusuario.insert(INSERT, user.usuario)

        self.txtsenha.delete(0, END)
        self.txtsenha.insert(INSERT,user.senha)



root = Tk()
Application(root)
root.mainloop()