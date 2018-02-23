#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fp = NULL;
    
    uint32_t code_size = 0;
    uint32_t loop_depth = 0;
    uint32_t loop_jumps[80] = {0};
    
    uint8_t tab[30000] = {0};
    uint8_t *cell = &tab[0];
    uint8_t *code = NULL;
    
    printf("Brainfuck Interpreter v1.0 (%s %s)\n", __DATE__, __TIME__);
    
    /* Open the source code file */
    if(argc == 2)
    {
        if((fp = fopen(argv[1], "r")) == NULL)
        {
            printf("Could not find the source code file [%s]\n", argv[1]);
            return EXIT_FAILURE;
        }
    }
    else if(argc == 1)
    {
        printf("Brainfuck Interpreter Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Get the size of the code in bytes */
    fseek(fp, 0, SEEK_END);
    code_size = ftell(fp);

    /* Allocate the memory for the code to run */
    if((code = malloc(code_size * sizeof(*code))) == NULL) {
        fprintf(stderr, "ERROR: Couldn't allocate the memory for code [%d bytes]\n", code_size);
        return EXIT_FAILURE;
    }
    
    printf("[Allocated %d bytes of memory for the program]\n", code_size);
    printf(">> ");

    /* Fill the memory with the code */
    fseek(fp, 0, SEEK_SET);
    fread(code, 1, code_size, fp);

    /* Close the file. We don't need it anymore */
    fclose(fp);
    
    /* Execute the code */
    for(uint32_t ip=0; ip<code_size; ip++)
    {
        switch(code[ip])
        {
            case '+':
                ++(*cell);
                if(*cell > 255) *cell = 0;
                break;
            
            case '-':
                --(*cell);
                if(*cell < 0) *cell = 255;
                break;
            
            case '>':
                ++cell;
                break;
            
            case '<':
                --cell;
                break;
            
            case '.':
                putchar(*cell);
                fflush(stdout);
                break;
            
            case ',':
                *cell = getchar() & 0xFF;
                fflush(stdin);
                break;
            
            case '[':
                loop_depth++;
                loop_jumps[loop_depth] = ip;
                break;
            
            case ']':
                if(*cell != 0)
                    ip = loop_jumps[loop_depth];
                else if(*cell == 0)
                    loop_depth--;
                break;
            
            case '#':
                printf("\n\n");
                for(uint8_t i=0; i<9; i++)
                printf("cell[%d] = %d\n", i, *(cell + i));
                break;
        }
    }

    free(code);
    code = NULL;
    
    putchar('\n');

    return EXIT_SUCCESS;
}
