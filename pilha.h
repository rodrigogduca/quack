#ifndef PILHA_H //verifica se ja existe e foi incluido a biblioteca da pilha, se ja tiver, nao inclui o codigo abaixo e pula pro "endif"
#define PILHA_H //abre a definicao da pilha 

#include <stdio.h>
#define MAX 100 //"troca o MAX no codigo por 100"


typedef int
    tp_item; //define tp_item como se fosse uma declaracao que eh um "int"


typedef struct {
    int topo; //so tipo int msm
    tp_item item[MAX]; //tipo de variavel tp_pilha, igual como se fosse int. e max trocado por 100 no define
} tp_pilha; //tp_pilha vira um tipo de variavel como int e float, so que eh do tipo dessa struct criada


//tipo de variavel tp_pilha e a variavel p com o * sendo o ponteiro que vai receber o local
void inicializa_pilha (tp_pilha *p){
    p -> topo = -1; //para usar ponteiro em variavel que ta numa struct usase "." tipo (quadra.bola), mas se for o primeiro no struct usa "->"
    //se recebesse a struct diretamente e nao o esdereco nao usava "->", seria usaro "p.topo"
}


//função para verificar se a pilha esta vazia --- recebe uma o endereco de uma variavel tipo tp_pilha, por usar ponteiro com p * 
int pilha_vazia (tp_pilha *p){ 
    if (p -> topo == -1) return 1;
return 0;
}


int pilha_cheia (tp_pilha *p){
    if (p -> topo == MAX-1){
        return 1;
    } else {
        return 0;
    }
}


//adicionar a pilha
int push (tp_pilha *p, tp_item e){ 
    if (pilha_cheia(p) == 1) return 0;

    p -> topo++;
    p -> item [p -> topo] = e;
    return 1;
}

//tirar da pilha
int pop (tp_pilha *p, tp_item *e){
    if (pilha_vazia(p)) return 0;
    *e=p->item[p->topo];
    p->topo--;
    return 1;
    }

    
//verificar topo
int top (tp_pilha *p, tp_item *e){
    if (pilha_vazia(p)) return 0;
    *e=p->item[p->topo];
    return 1;
}

//imprime a poliha e nao usa * na entrada pois coipia a pilha e nao usa ponteiro
void imprime_pilha (tp_pilha p){
    tp_item e;
    printf("\n");
    while (!pilha_vazia(&p)){
        pop(&p, &e); //aqui usa & para pegar espaço de memoria 
        printf("%d ", e);
    }
}
  

int altura_pilha (tp_pilha *p){
    return p->topo+1;
}


void retira_impares (tp_pilha *p){
    tp_pilha paux;//cria uma pilha auxiliar
    inicializa_pilha (&paux); //inicializa a pilha e tira o lixo com o topo sendo -1
    tp_item e;
    
    while (!pilha_vazia(p)){
        pop(p, &e); //so o p pq ja é endereco do ponteiro, o "e" tem & pq eh p passar o endereco do e para salvar la
        if (e%2==0) //teste se o valor de "e" (oq foi retirado da pilha)  for par
        push(&paux, e); //joga o valor que eh um par para a pilha auxiliar
        
        //se nao for par, ele ja tirou da pilha, ent o proximo sobrepoe o o valor se perde se for impar    !!!!!
    }
    while (!pilha_vazia(&paux)){//enqaunto a a pilha auxiliar nao tiver vazia segue desempilhando
        pop(&paux, &e);//pega o primeiro item da pilha auxiliar e joga pra variavel "e"
        push(p, e); //pega esse item da variavel "e" e joga de volta para "p", que nesse caso ja eh o ponteiro da pilha
    }
}


int pilhas_iguais(tp_pilha p1, tp_pilha p2){
    tp_item e1, e2;
    
    if (altura_pilha(&p1) != altura_pilha(&p2))
    return 0;
    
    while (!pilha_vazia(&p1)){
        pop(&p1, &e1);
        pop(&p2, &e2);
        if (e1 != e2) return 0;
    }
    return 1;
}


int empilha_pilhas(tp_pilha *p1, tp_pilha *p2){
    tp_item e;
    tp_pilha paux;
    
    inicializa_pilha(&paux);
    
    if ( altura_pilha(p1) + altura_pilha(p2) > MAX)
    return 0;
    
    while (!pilha_vazia(p2)){
        pop(p2, &e);
        push(&paux, e); }
        while (!pilha_vazia(&paux)){
            pop(&paux, &e);
            push(p1, e); }
            return 1;
            
        }


#endif //finaliza definicao da pilha, em caso de ja ter sido incluida, pula pra ca e nao inclui novamente esse codigo superior