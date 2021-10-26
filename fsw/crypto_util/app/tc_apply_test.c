#include "tc_apply_test.h"

int main(int argc, char *argv[]) {

    //Setup & Initialize CryptoLib
    Crypto_Init();
    fprintf(stdout,"Crypto engine is running.\n");

    // setup a loop allowing either a structure to be iterated over (or lines of a file), or to allow cmdline entry

    char *buffer;
    char const *filename1;
    char const *filename2;
    char const *filename3;
    // char const *filename4;
    long buffer_size;
    char *security_type;

    if (argc == 5 ) {
        security_type = argv[1];
        filename1 = argv[2];
        filename2 = argv[3];
        filename3 = argv[4];
        // filename4 = argv[5];
    } 

    else {
        fprintf(stderr,"Command line usage: \n"\
               "\t%s <tc|tm|aos> \n"\
               "<tc|tm|aos> : Process TeleCommand (tc) | Telemetry (tm) | Advanced Orbiting Systems (aos) Security T\n"\
               ,argv[0]);

        return OS_ERROR;
    }


    buffer = c_read_file(filename1,&buffer_size);
    debug_printf("File buffer size:%lu\n",buffer_size);
    int buffer_size_i = (int) buffer_size;
    debug_printf("File buffer size int:%d\n",buffer_size_i);
    debug_printf("File content: \n");
    debug_hexprintf(buffer,buffer_size_i);

    Crypto_TC_ProcessSecurity(buffer, &buffer_size_i);

    sleep(1);

    buffer = c_read_file(filename2,&buffer_size);
    debug_printf("File buffer size:%lu\n",buffer_size);
    buffer_size_i = (int) buffer_size;
    debug_printf("File buffer size int:%d\n",buffer_size_i);
    debug_printf("File content: \n");
    debug_hexprintf(buffer,buffer_size_i);


    Crypto_TC_ProcessSecurity(buffer, &buffer_size_i);

    sleep(1);

    buffer = c_read_file(filename3,&buffer_size);
    debug_printf("File buffer size:%lu\n",buffer_size);
    buffer_size_i = (int) buffer_size;
    debug_printf("File buffer size int:%d\n",buffer_size_i);
    debug_printf("File content: \n");
    debug_printf("Finished loading test raw TC frame...\n");
    debug_hexprintf(buffer,buffer_size_i);


    Crypto_TC_ApplySecurity(&buffer, &buffer_size_i);

    free(buffer);
}