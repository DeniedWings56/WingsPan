# proj_23-24

O projeto **IST-KVS** implementa um sistema de chave-valor que permite operações monitoradas e acessíveis a processos clientes. Ele é dividido em duas partes principais:  
1. Tornar o IST-KVS acessível a processos clientes por meio de **named pipes**, permitindo que os clientes monitorem alterações em pares chave-valor.  
2. Implementar a finalização das conexões entre o IST-KVS e os clientes utilizando **sinais**.

Foi incluída uma flag `int KVS_LOGGING = 0;` no código main.c do server, que pode ser ativada, para habilitar o registro de todas as operações realizadas no IST-KVS.