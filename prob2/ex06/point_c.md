**Alinea c**

- Para criar um hardlink:
`ln temp.txt temp1.txt`

- Para criar um symbolic link:
`ln -s temp.txt temp2.txt`


Depois de criar os links podemos executar o programa da alinea b) e verificar que:
- temp.txt e temp1.txt têm o mesmo tipo (regular) mesmo tamanho e mesmo i-node;
- temp2.txt não tem o mesmo i-node e é do tipo (other).