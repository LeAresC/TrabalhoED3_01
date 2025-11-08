void escreveCabecalhoIndice(FILE *arquivoIndice, char status) {
    // Cria o cabeçalho do arquivo de índice
    CabecalhoIndice dataHeader;
    dataHeader.status = status;
    memset(dataHeader.lixo, '$', sizeof(dataHeader.lixo));

    // Escreve o cabeçalho no arquivo de índice
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoIndice);
    fwrite(dataHeader.lixo, sizeof(dataHeader.lixo), 1, arquivoIndice);
}

void escreveCabecalhoPessoa(FILE *arquivoPessoa, char status, int quantidadePessoas, int quantidadeRemovidos, long long int proxByteOffset) {
    // Cria o cabeçalho do arquivo de pessoas
    CabecalhoPessoa dataHeader;
    dataHeader.status = status;
    dataHeader.quantidadePessoas = quantidadePessoas;
    dataHeader.quantidadeRemovidos = quantidadeRemovidos;
    dataHeader.proxByteOffset = proxByteOffset;

    // Escreve o cabeçalho no arquivo de pessoas
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoPessoa);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoPessoa);
    fwrite(&dataHeader.quantidadeRemovidos, sizeof(dataHeader.quantidadeRemovidos), 1, arquivoPessoa);
    fwrite(&dataHeader.proxByteOffset, sizeof(dataHeader.proxByteOffset), 1, arquivoPessoa);
}

void escreveCabecalhoSegue(FILE *arquivoSegue, char status, int quantidadePessoas, long long int proxRRN) {
    // Cria o cabeçalho do arquivo de segue
    CabecalhoSegue dataHeader;
    dataHeader.status = status;
    dataHeader.quantidadePessoas = quantidadePessoas;
    dataHeader.proxRRN = proxRRN;

    // Escreve o cabeçalho no arquivo de segue
    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoSegue);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoSegue);
    fwrite(&dataHeader.proxRRN, sizeof(dataHeader.proxRRN), 1, arquivoSegue);
}

CabecalhoSegue leCabecalhoSegue(FILE *arquivo) {
    // Le o cabeçalho do arquivo de dados
    CabecalhoSegue cabecalho;
    fread(&cabecalho.status, sizeof(char), 1 , arquivo);
    fread(&cabecalho.quantidadePessoas, sizeof(int), 1 ,arquivo);
    fread(&cabecalho.proxRRN, sizeof(long long int), 1, arquivo);
    return cabecalho;
}
