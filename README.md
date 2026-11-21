# Sistema de Gestão de Estacionamento

**Disciplina:** Lógica de Programação e Algoritmos — **Semestre:** 2026/2
**Trabalho:** Trabalho do 2º Bimestre (B2) — Sistema de Gestão de Estacionamento, em C, executado em terminal

## Integrantes

| Nome | Perfil GitHub |
|---|---|
| Tarcísio Lima de Oliveira Rocha | [@limadeoliveirarochatarcisio-creator](https://github.com/limadeoliveirarochatarcisio-creator) |
| Sthener Vieira Campos de Mello | [@sthener](https://github.com/sthener) |

## Cenário e escopo

O estacionamento fictício tem 4 fileiras com 5 vagas cada (20 vagas). O programa começa sem veículos, recebe operações por um menu e mantém os dados apenas enquanto está em execução (não há gravação em arquivos). Todos os identificadores e descrições usados nos testes são fictícios.

## Funcionalidades e menu

```
1 - Cadastrar veiculo
2 - Consultar veiculo por ID
3 - Atualizar descricao ou categoria
4 - Mover veiculo para outra vaga
5 - Remover veiculo
6 - Listar veiculos cadastrados
7 - Exibir mapa de vagas
8 - Exibir relatorio da ocupacao
0 - Encerrar
```

- **Cadastro:** valida identificador (7 caracteres A–Z/0–9, minúsculas viram maiúsculas), descrição (não vazia, sem ser só espaços, até 60 caracteres), categoria (1 = carro, 2 = moto), fileira (1–4) e coluna (1–5). Recusa ID duplicado (sem distinção de caixa), vaga ocupada e lotação (20 vagas). Se qualquer validação falhar, nada é gravado.
- **Consulta:** mostra identificador, descrição, categoria e posição; informa quando não localizado.
- **Atualização:** altera descrição e categoria, preservando ID e vaga; valida antes de confirmar.
- **Movimentação:** valida o destino; libera a origem e ocupa o destino; informa se o destino é a própria vaga; recusa destino ocupado sem alterar nada.
- **Remoção:** libera a vaga e remove o registro deslocando os elementos seguintes (sem lacunas).
- **Listagem, mapa e relatório:** o relatório mostra total, ocupadas, livres, percentual (duas casas decimais), carros e motos, sempre pelo estado atual.
- **Encerramento (0):** libera a memória dinâmica. Se a entrada de dados acabar (fim de arquivo), o programa também encerra liberando a memória.

## Estruturas utilizadas

- **Mapa de vagas:** matriz `int mapa[4][5]`; `0` = livre, `1` = ocupada. Percorrida com laços aninhados. Na tela, fileiras/colunas começam em 1; internamente usam-se índices 0–3 e 0–4 (conversão `f - 1`, `col - 1`).
- **Registro (`struct Veiculo`):** `id` (7 caracteres + `\0`), `descricao` (60 + `\0`), `categoria`, `fileira`, `coluna`.
- **Coleção dinâmica (`struct Colecao`):** ponteiro `dados` para um vetor de `Veiculo`, `qtd` (registros em uso) e `capacidade` (espaço reservado). Começa com capacidade 2 (`malloc`); quando `qtd == capacidade`, a capacidade é **dobrada com `realloc`**, usando um ponteiro temporário: se falhar, o cadastro é cancelado e a coleção e o mapa permanecem íntegros. A capacidade da lista não altera o número de vagas (20). A remoção não reduz a capacidade; o `free` ocorre ao encerrar.
- **Consistência:** a quantidade de registros ativos é sempre igual ao número de células `1` na matriz e às vagas ocupadas do relatório.

## Modularização

Projeto em **um único arquivo** (`estacionamento.c`), escolha feita pela simplicidade do trabalho. Funções (além da `main`), sem variáveis globais; o estado é passado por parâmetros (ponteiro para a coleção e a matriz):

| Responsabilidade | Funções |
|---|---|
| Mapa | `iniciar_mapa`, `exibir_mapa` |
| Coleção/memória | `iniciar_colecao`, `liberar_colecao` |
| Entrada e validação | `limpar_buffer`, `ler_linha`, `descricao_valida`, `para_maiusculo`, `id_valido` |
| Busca | `buscar_veiculo` |
| Operações do menu | `cadastrar_veiculo`, `consultar_veiculo`, `atualizar_veiculo`, `mover_veiculo`, `remover_veiculo`, `listar_veiculos`, `exibir_relatorio` |

## Compilação e execução

```
gcc -std=c11 -Wall -Wextra -pedantic estacionamento.c -o estacionamento
./estacionamento
```

Compila sem avisos. Também foi testado com `-fsanitize=address,undefined` (sem erros de memória nem vazamentos).

## Casos de teste

| Teste | Operação / entrada | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|
| T01 – Vazio | Abrir e escolher 7, 6 e 8 | Mapa livre; listagem vazia; ocupação 0,00% | Mapa todo `.`; "Não há veículos cadastrados"; 0,00% | Aprovado |
| T03 – Duplicidade sem caixa | Cadastrar `ABC1D23` e depois `abc1d23` | Segundo cadastro recusado | "Já existe um veículo com esse identificador"; contagem inalterada | Aprovado |
| T05 – Crescimento dinâmico | Cadastrar 3 veículos (capacidade inicial 2) | Terceiro cadastro amplia a coleção | 3 veículos cadastrados, sem erro (verificado com AddressSanitizer) | Aprovado |
| T06 – Atualização | Opção 3 em `ABC1D23`: "Moto de entrega", categoria 2 | Dados mudam sem duplicar registro | Listagem com 1 registro, categoria Moto, nova descrição | Aprovado |
| T07 – Movimentação | `ABC1D23` de F1/C1 para F2/C3; antes, tentativa para F1/C2 ocupada | Destino ocupado recusado; movimento válido libera origem e ocupa destino | Recusa em F1/C2 sem alterar; depois mapa com X em F2/C3 e F1/C1 livre | Aprovado |
| T08 – Remoção | Remover `XYZ9K87` (F1/C2) | Registro some e vaga é liberada | "Veículo removido e vaga F1/C2 liberada"; relatório com 2 ocupadas | Aprovado |
| T09 – Lotação | Cadastrar 20 veículos e tentar o 21º | 21º recusado | "Estacionamento lotado (20 vagas ocupadas)"; relatório 20/0/100,00% | Aprovado |
| T10 – Encerramento | Opção 0 (e fim de entrada) | Memória liberada | Encerra normalmente; AddressSanitizer sem vazamentos | Aprovado |
| Cenário 7.1 | Sequência completa do roteiro | 20 vagas; 2 ocupadas; 18 livres; 10,00%; 2 carros; 0 motos | Exatamente esses valores | Aprovado |
| Validações | ID com tamanho ≠ 7, com espaço/símbolo; descrição só com espaços ou com mais de 60 caracteres; fileira/coluna fora do intervalo | Rejeitar sem alterar o estado | Rejeitados; nenhum cadastro parcial | Aprovado |

## Distribuição das responsabilidades e integração das branches

Conforme o histórico do repositório:

- **Tarcísio:** base do programa (matriz de vagas, coleção dinâmica, menu), cadastro, consulta e listagem — branches `feature/cadastro` e `fatura/consulta-listagem`.
- **Sthener:** movimentação, atualização, remoção e relatório de ocupação — branch `feature/movimentacao-relatorio`.
- **Integração:** as três branches foram integradas à `main` por Pull Requests (#1, #2 e #3), sem conflitos de merge.
- **Revisão final:** correção da ordem do menu (3 = atualizar, 4 = mover, conforme o roteiro), tratamento de fim de entrada, validação de descrição só com espaços/linha longa, `.gitignore` e este README.

## Limitações conhecidas

- Os dados existem só durante a execução (sem persistência em arquivo, como previsto no roteiro).
- Descrição lida por linha: caracteres acentuados ocupam mais de um byte, então o limite de 60 conta bytes, não letras.
- O identificador é lido até o primeiro espaço; um ID com espaço é rejeitado por ter tamanho diferente de 7.

## Declaração individual de autoria e uso de IA

### Tarcísio Lima de Oliveira Rocha

- **(a) Funcionalidades em que atuei:** estrutura inicial do programa (matriz de vagas, coleção dinâmica, menu), cadastro, consulta de veículo por ID e listagem (branches `feature/cadastro` e `fatura/consulta-listagem`), integradas à `main` por Pull Requests.
- **(b) Usei IA generativa?** Sim.
- **(c) Ferramentas, finalidade e exemplos:**
  - **Gemini:** dúvidas sobre o fluxo do GitHub pela interface web (criar/editar arquivos em branch, commits, abrir e mesclar Pull Requests), conferência de requisitos do roteiro e estrutura da documentação. Exemplos de pedidos: "confirma? ou coloca criar outra branch?", "chat a minha parte tem que colocar que eu usei IA no README né, pq tá no PDF" e "quero que você faça a minha parte da declaração como o PDF pediu".
  - **Claude (Anthropic):** na revisão final. Conferiu o repositório contra o roteiro, testou o programa e produziu as correções do código (ordem do menu, tratamento de fim de entrada, validação de descrição), o `.gitignore` e o texto deste README. Exemplos de pedidos: "consegue acessar o link e ver se o trabalho está de acordo com o PDF?" e "faz tudo aí para mim".
  - **Código das minhas funções:** desenvolvi as funções da minha parte com apoio de IA para tirar dúvidas, revisar a lógica e ajustar detalhes do código, conferindo o resultado antes de confirmar os commits.
  - **Testes e revisão:** li o código antes de subir e acompanhei a integração no repositório; antes da entrega final, vou conferir os testes para garantir que tudo bate com o roteiro.
- **(d) Adaptação de código externo?** Não houve adaptação de código de terceiros além do material da disciplina.

### Sthener Vieira Campos de Mello (@sthener)

- **(a) Funcionalidades:** movimentação de veículo entre vagas, atualização de descrição/categoria, remoção de veículo e relatório de ocupação.
- **(b) Utilizou IA generativa?** Sim.
- **(c) Ferramentas, solicitações e sugestões incorporadas:** usei o Gemini para tirar dúvidas, revisar a lógica das funções e montar o README. Exemplos de solicitações: conferir a lógica da movimentação de vagas e ajustar o texto da declaração no README. Incorporei revisões de validação e a organização da documentação.
- **(d) Adaptação de código externo e fontes:** não houve adaptação de código externo; utilizei apenas o material da disciplina e a documentação padrão de C.

## Referências externas

Material da disciplina e documentação padrão da linguagem C. Nenhum código externo foi adaptado.
