**Alinea d**

Quando mudamos a chamada lstat() para stat(), o ficheiro original (temp.txt), o hardlink (temp1.txt) e o 
symbolic link (temp2.txt) têm o mesmo tipo, i-node e tamanho. Isto acontece porque quando stat() recebe
um pathname que é symbolic link, retorna informação sobre o ficheiro original, e não do link. Já lstat()
retornaria informação sobre o link e não sobre o ficheiro original.

*TiagooGomess*