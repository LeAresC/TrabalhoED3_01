void escreveCabecalhoIndice(FILE *arquivoIndice, char status) {
    CabecalhoIndice dataHeader;
    dataHeader.status = status;
    memset(dataHeader.lixo, '$', sizeof(dataHeader.lixo));

    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoIndice);
    fwrite(dataHeader.lixo, sizeof(dataHeader.lixo), 1, arquivoIndice);
}

void escreveCabecalhoPessoa(FILE *arquivoPessoa, char status, int quantidadePessoas, int quantidadeRemovidos, long long int proxByteOffset) {
    CabecalhoPessoa dataHeader;
    dataHeader.status = status;
    dataHeader.quantidadePessoas = quantidadePessoas;
    dataHeader.quantidadeRemovidos = quantidadeRemovidos;
    dataHeader.proxByteOffset = proxByteOffset;

    fwrite(&dataHeader.status, sizeof(dataHeader.status), 1, arquivoPessoa);
    fwrite(&dataHeader.quantidadePessoas, sizeof(dataHeader.quantidadePessoas), 1, arquivoPessoa);
    fwrite(&dataHeader.quantidadeRemovidos, sizeof(dataHeader.quantidadeRemovidos), 1, arquivoPessoa);
    fwrite(&dataHeader.proxByteOffset, sizeof(dataHeader.proxByteOffset), 1, arquivoPessoa);
}

void escreveCabecalhoSegue(FILE *arquivoSegue, char status, int quantidadePessoas, long long int proxRRN) {
    CabecalhoSegue dataHeader;
    dataHeader.status = status;
    dataHeader.quantidadePessoas = quantidadePessoas;
    dataHeader.proxRRN = proxRRN;

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
