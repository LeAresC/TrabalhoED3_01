import struct

# Abre o arquivo em modo de escrita binária ('wb')
with open('indexaPessoa.bin', 'wb') as f:
    # --- 1. Escreve o Registro de Cabeçalho (12 bytes) ---
    # status ('1') seguido por 11 bytes de lixo ('$')
    f.write(b'1')
    f.write(b'$' * 11)

    # --- 2. Escreve os Registros de Dados do Índice (12 bytes cada) ---
    # Os dados são (idPessoa, byteOffset)
    
    # --- Registro de Índice para idPessoa = 10 ---
    # O registro de dados correspondente começa no byte 17 do arquivo pessoa.bin
    f.write(struct.pack('<i', 10)) # idPessoa (4 bytes)
    f.write(struct.pack('<q', 17)) # byteOffset (8 bytes)

    # --- Registro de Índice para idPessoa = 20 ---
    # O registro de dados correspondente começa no byte 92 do arquivo pessoa.bin
    f.write(struct.pack('<i', 20)) # idPessoa (4 bytes)
    f.write(struct.pack('<q', 92)) # byteOffset (8 bytes)

    # --- Registro de Índice para idPessoa = 25 ---
    # O registro de dados correspondente começa no byte 137 do arquivo pessoa.bin
    f.write(struct.pack('<i', 25)) # idPessoa (4 bytes)
    f.write(struct.pack('<q', 137))# byteOffset (8 bytes)

print("Arquivo 'indexaPessoa.bin' criado com sucesso!")