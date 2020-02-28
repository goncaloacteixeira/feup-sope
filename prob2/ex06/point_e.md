**Alinea e**

Ao apagarmos o ficheiro temp.txt ainda conseguimos aceder e ver o conteudo de temp1.txt; isto acontece porque um *hardlink* contém o conteudo do ficheiro original;

No entanto o ficheiro temp2.txt não podemos ver o seu conteudo com o comando `cat`, uma vez que um *symbolic link* é como um apontador para o ficheiro original, uma vez que este deixa de existir também a utilidade o ficheiro temp2.txt deixa de ser relevante.