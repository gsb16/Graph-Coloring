Trabalho de implementação 2
===========================

CI065 Algoritmos e Teoria dos Grafos - Junho de 2018

Autores
~~~~~~~
Gabriel de Souza Barreto
Guilherme Bastos de Oliveira

Relatório
~~~~~~~~~
Readme com as informações relevantes relacionadas ao trabalho 2 de Grafos. O objetivo do trabalho foi implementar as funções especificadas e funções adicionais para realizar a coloração de grafos.

Sobre a estrutura de dados struct grafo
---------------------------------------
- Como a biblioteca cgraph estava a disposição e oferece uma estrutura de grafos completa para as necessidades deste trabalho a struct grafo contem apenas um ponteiro para uma varíavel do tipo Agraph_t.

Sobre a estrutura de dados struct vertice
-----------------------------------------
- É um record para uso com a biblioteca cgraph, por isso possui como header Agrec_t. Contém um ponteiro para o vetor de int que armazena o rótulo, o valor da cor do vértice, o tamanho atual do rótulo e um ponteiro para o vértice na estrutura da cgraph.

Sobre estrutura adicional struct elemento
-----------------------------------------
- Estrutura simples para implementação de uma lista encadeada.

Sobre funções adicionais
------------------------
- addNaLista() - recebe como parâmetros uma cabeça de lista e um vértice, insere o vértice no final da lista.
- adicionaNoRotulo() - recebe como parâmetros um vértice e um valor, acrescenta o valor ao rótulo.
- comparaRotulo() - recebe como parâmetros 2 vértices, retorna o resultado da comparação entre eles (primeiro - segundo).
- verticeMaiorRotulo() - recebe como parâmetro a cabeça de uma lista, retorna o vértice de maior rótulo e o remove da lista.
- corGulosa() - recebe como parâmetros um grafo e um nodo, retorna uma cor para o vértice de maneira gulosa (o menor número disponível considerando os vizinhos)

Sobre as funções implementadas
------------------------------
- le_grafo() - aloca uma struct grafo e chama a função agread() da biblioteca cgraph que realiza a leitura de um grafo em .dot dado um ponteiro para um arquivo. Após completar a leitura adiciona um record a cada vértice do grafo utilizando a função agbindrec() e aloca vetor para rótulos.
- escreve_grafo() - calcula cor em formato #rrggbb para cada vértice e faz uma chamada para a função agwrite() da biblioteca cgraph que imprime um grafo em formato .dot dado um ponteiro para um arquivo de saída.
- destroi_grafo() - libera a memória utilizada para armazenar os rótulos e realiza a chamada para a função agclose() da biblioteca cgraph que libera a memória utilizada pela estrutura de grafos da própria biblioteca e libera a memória utilizada pela struct grafo. Sempre devolve 1 ao final pois não é possível decidir se a liberação de memória foi realizada com sucesso.
- n_vertices() - retorna o número de vértices de um grafo, calculado através da função agnnodes().
- vertice_de_nome() - retorna o vértice cujo nome é o passado por parâmetro.
- busca_lexicografica() - armazena todos os vértices em um lista, para cada iteração encontra o vértice v de maior rótulo, remove da lista, adiciona o tamanho da lista nos rótulos dos vizinhos de v, e insere v na posição tamanho da lista do vetor de ordem lexicográfica. O vértice inicial é passado como parâmetro.
- cor() - acessa o valor da cor do vértice na struct vertice e o retorna.
- colore() - para todos os vértices chama a função cor() e atribui o resultado ao vértice.

Sobre as estruturas e funções da biblioteca cgraph
--------------------------------------------------
- Agraph_t - grafo da biblioteca, possui vértices e arestas.
- Agedge_t - arestas, possuem nome e no grafo de recomendações o atributo peso.
- Agnode_t - vértices, possuem nome e atributo tipo.
- agattr() - cria atributo para um  dos tipos dos dados da biblioteca (grafo, aresta ou vértice) - utilizada para criar o atributo color.
- agset() - dados um objeto, um atributo e um valor, define o para o objeto o valor do atributo informado - utilizada para armazenar as cores em #rrggbb.
- agfst{node, edge}() e agnxt{node, edge}() - retornam o primeiro vértice/aresta de acordo com a organização interna da biblioteca e o próximo vértice/aresta, respectivamente - utilizadas para percorrer o grafo.
- agbindrec() - associa um record de estrutura definada pelo programador a um dos tipos de dados da biblioteca (grafo, aresta ou vértice) - utilizada para associar a estrutura struct vertice aos vértices da cgraph.
- aggetrec() - retorna o ponteiro para a estrutura associada ao elemento - utilizada para acessar a struct vertice de cada vértice.
- agnnodes() - retorna o número de vértices que um grafo possui.

Sobre peculiaridades da implementação
-------------------------------------
- Casting (char*)(unsigned long) nos tipos const char * - um artifício técnico foi utilizado para suprimir a geração de warnings relacionados as flags -Wdiscarded-qualifiers e -Wcast-qual, o casting serve apenas para que esses warnings não sejam levantados pelo compilador.
- g - g; na função cor() - um artifício técnico foi utilizado para suprimir a geração de warnings relacionados a flag -Wunused-parameter, a atribuição serve apenas para que esses warnings não sejam levantados pelo compilador.
