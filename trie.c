// C implementation of search and insert operations
// on Trie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 

#define ALPHABET_SIZE (26)
 

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
 

struct TrieNode
{
     int children[ALPHABET_SIZE];
 
    bool isLeaf;
};
 

int getNode(FILE *treeFile)
{
	int registro;
    struct TrieNode pNode;
    

    memset(&pNode, 0, sizeof(struct TrieNode));
    fwrite(&pNode, sizeof(struct TrieNode),1, treeFile);
    fseek(treeFile, 0, SEEK_END);


    registro = (int)((ftell(treeFile)/sizeof(struct TrieNode))-1);;


    return registro;
}

int insert(const char key[], FILE *treeFile)
{
	int index;
	int i;
    int level = 0;
    int length = strlen(key);
    int novoNO;
    int regAtual = 0;
    struct TrieNode node;
 
    fseek(treeFile, 0, SEEK_SET);
 	fread(&node,sizeof(struct TrieNode), 1, treeFile);

	if(CHAR_TO_INDEX(key[level]) == 0) {
		for(level = 0; level < length; level++){
			index = CHAR_TO_INDEX(key[level]);
			novoNO = getNode(treeFile);
			fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
			fread(&node, sizeof(struct TrieNode), 1, treeFile);

			index = novoNO;
			fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
			fwrite(&node, sizeof(struct TrieNode), 1, treeFile);

			regAtual = novoNO;
			}

		fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
		fread(&node, sizeof(struct TrieNode), 1, treeFile);
		node.isLeaf = true;
		fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
		fwrite(&node, sizeof(struct TrieNode), 1, treeFile);

		return 0;
	} else if (index != 0) {
		while (CHAR_TO_INDEX(key[level]) != 0){
			if(node.isLeaf == true && level == (strlen(key)-1)){
				return 1;
			}
		}

		if(node.isLeaf == false && level == (strlen(key)-1)){
			fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
			fread(&node, sizeof(struct TrieNode), 1, treeFile);
			node.isLeaf = true;
			fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
			fwrite(&node, sizeof(struct TrieNode), 1, treeFile);

			return 0;
		}
		regAtual = CHAR_TO_INDEX(key[level]);
		fseek(treeFile, 0, SEEK_SET);
 		fread(&node,sizeof(struct TrieNode), 1, treeFile);
 		level ++;
	}

	for(i = level; i<strlen(key); i++){
		index = CHAR_TO_INDEX(key[i]);
		novoNO = getNode(treeFile);
		fseek(treeFile, 0, SEEK_SET);
 		fread(&node,sizeof(struct TrieNode), 1, treeFile);
 		index = novoNO;
 		fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
		fwrite(&node, sizeof(struct TrieNode), 1, treeFile);

		regAtual = novoNO;
	}

	fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
	fread(&node, sizeof(struct TrieNode), 1, treeFile);
	node.isLeaf = true;
	fseek(treeFile, regAtual*sizeof(struct TrieNode), SEEK_SET);
	fwrite(&node, sizeof(struct TrieNode), 1, treeFile);

	return 0;

 }


void getWords(FILE *entrada) {
	int c;
	char vector[99];
	int i=0;

	c = fgetc(entrada);

	while(c != EOF){
		while(isalpha(c) && c != ' ') {
			vector[i]=c;
			i++;
			c=fgetc(entrada);
		}
		insert(vector, entrada);
		memset(vector, 0, 99);
		i=0;
		c=fgetc(entrada);
	}
}

void cleanWords(FILE *entrada, FILE *saida){
	int c;

	c=fgetc(entrada);

	while(c != EOF){
		if(isalpha(c)){
			c=tolower(c);
			fputc(c, saida);
		}
		if(c=='\n')
			fputc(c, saida);
		if(c==' ')
			fputc(c, saida);
		if(c=='\t')
			fputc(c, saida);
		
        c = getc(entrada);
	}
}
 

int main()
{
	int i;
	int c;
	char *p;
	char *keys;
	FILE *entrada, *saida, *treeFile, *novaEntrada;

	saida = fopen("renato_saida.txt","w"); //abre o arquivo que será escrito após a edição de carecteres

    if(!saida)
    {
        fprintf(stderr,"Arquivo %s não pode ser aberto para escrita\n", "renato_saida.txt");
        return 1;
    }

    entrada = fopen("renato.txt", "r"); //abre o arquivo contendo o livro

	if(!entrada)
	{
		fprintf(stderr,"Arquivo %s não encontrado\n", "renato.txt");
		fclose(saida);
		return 1;
	}

	cleanWords(entrada, saida); //retira caracteres indesejados do arquivo de entrada

	fclose(entrada);
	fclose(saida);

	novaEntrada = fopen("renato_saida.txt", "r");

	if(!novaEntrada)
    {
        fprintf(stderr,"Arquivo %s não pode ser aberto para escrita\n", "renato_saida.txt");
        return 1;
    }

    treeFile = fopen("treeFile.dat", "w+");

    if(!treeFile)
    {
        fclose(novaEntrada);
        fprintf(stderr,"Arquivo %s não pode ser aberto para escrita\n", "treeFile.dat");
        return 1;
    }

    getNode(treeFile);

    printf("Teste pós getNode \n");

    getWords(novaEntrada);

	fclose(novaEntrada);
	fclose(treeFile);

	return 0;
}