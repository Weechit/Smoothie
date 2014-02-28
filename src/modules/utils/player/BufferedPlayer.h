/*
	ZMorph custom code
*/


#ifndef BUFFEREDPLAYER_H
#define BUFFEREDPLAYER_H

#include "libs/Kernel.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "libs/StreamOutput.h"

#define bufferedplay_command_checksum           CHECKSUM("bufferedplay")

class BufferedPlayer : public Module {
	public:
		BufferedPlayer(){}
		
	    void on_module_loaded();
	    void on_console_line_received( void* argument );
	    void on_main_loop( void* argument );
	    void on_second_tick(void* argument);
	    void on_gcode_received(void *argument);
	    void start_playing();

	private:	
	    string filename;
	    bool playing_file;
	    StreamOutput* current_stream;
	    StreamOutput* reply_stream;
	    FILE* current_file_handler;
        unsigned long elapsed_secs;

        int currentpos;
};






#endif