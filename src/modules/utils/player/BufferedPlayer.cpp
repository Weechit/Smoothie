#include "BufferedPlayer.h"

#include "libs/Kernel.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"
#include "libs/SerialMessage.h"
#include "libs/StreamOutput.h"
#include "modules/robot/Conveyor.h"
#include "DirHandle.h"
#include "PublicDataRequest.h"


void BufferedPlayer::on_module_loaded(){
    this->playing_file = false;
    this->register_for_event(ON_CONSOLE_LINE_RECEIVED);
    this->register_for_event(ON_MAIN_LOOP);
    this->register_for_event(ON_SECOND_TICK);
    this->register_for_event(ON_GCODE_RECEIVED);

    this->elapsed_secs= 0;
    this->reply_stream= NULL;
    this->filename = "/sd/voxoutput";
    this->current_stream = &(StreamOutput::NullStream);
}

void BufferedPlayer::on_second_tick(void*) {
     if (!THEKERNEL->pauser->paused()) this->elapsed_secs++;
}

void BufferedPlayer::on_gcode_received(void *argument) {
    Gcode *gcode = static_cast<Gcode*>(argument);

    // some pause support would be nice

}


void BufferedPlayer::on_console_line_received( void* argument ){
    SerialMessage new_message = *static_cast<SerialMessage*>(argument);

    // ignore comments
    if(new_message.message[0] == ';') return;

    string possible_command = new_message.message;

	unsigned short check_sum = get_checksum( possible_command.substr(0,possible_command.find_first_of(" \r\n")) );

	if (check_sum == bufferedplay_command_checksum )
		this->start_playing();
    // add
}


void BufferedPlayer::start_playing()
{
    //if(this->current_file_handler == NULL){
        //stream->printf("File not found: %s\r\n", this->filename.c_str());
       // return;
    //}

    this->currentpos = 0;
	this->playing_file = true;
}



void BufferedPlayer::on_main_loop(void* argument)
{
	//if (!this->playing_file)
	//	this->start_playing();

	if (this->playing_file)
	{
		// first we open the file, if it's impossible we just return

    	this->current_file_handler = fopen( this->filename.c_str(), "r");
    	if(this->current_file_handler == NULL)
    		return;

    	int result = fseek(this->current_file_handler, currentpos, SEEK_SET);
    	if (0 != result) {//this should be handled somehow 
    		this->currentpos = 0;
    		return;
    	}

        string buffer;
        bool discard= false;
        int c;
        buffer.reserve(20);
        // Print one line of the file ()
        while ((c = fgetc(this->current_file_handler)) != EOF){
        	currentpos++;
            if (c == '\n'){
                if(discard) {
                    // we hit a long line and discarded it
                    discard= false;
                    buffer.clear();
                    this->current_stream->printf("Warning: Discarded long line\n");
                    return;
                }
                this->current_stream->printf("%s\n", buffer.c_str());
                struct SerialMessage message;
                message.message = buffer;
                message.stream = &(StreamOutput::NullStream); // we don't really need to see the ok
                // wait for the queue to have enough room that a serial message could still be received before sending
                THEKERNEL->conveyor->wait_for_queue(2);
                THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message);
                //played_cnt += buffer.size();
                buffer.clear();
                fclose(this->current_file_handler);
                return;

            }else if(buffer.size() > 128) {
                // discard rest of line
                discard= true;

            }else{
                buffer += c;
            }
        }
        fclose(this->current_file_handler);
	}
}