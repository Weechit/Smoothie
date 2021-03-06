#ifndef _FILESTREAM_H_
#define _FILESTREAM_H_

#include "StreamOutput.h"
#include "stdlib.h"

class FileStream : public StreamOutput {
    public:
        FileStream(const char *filename) { fd= fopen(filename, "a"); }
        virtual ~FileStream(){ close(); }
        int puts(const char *str){ return (fd == NULL) ? 0 : fwrite(str, 1, strlen(str), fd); }
        void close() { if(fd != NULL) fclose(fd); fd= NULL; }

    private:
        FILE *fd;
};

#endif
