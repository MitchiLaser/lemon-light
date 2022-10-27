/**
 * Firmware for the lemon-light project
 * 
 * Frank Hermann, 25 Oct 2022
 */

#include <Homie.h>

#define FW_NAME    "lemon-light-contoler"
#define FW_VERSION "1.0.0"
#define BRAND      "A4-IOT"


enum error_type{
  LINE_TO_LONG,
  LINE_TO_SHORT,
  BUFFER_FULL,
  UNKNOWN_SYMBOL,
  NO_ERROR
};

struct box_pos{
  int col, row;
  bool flip_cols, flip_rows;
};

HomieNode node("lemon-light", "Display", "lemon-light");
constexpr unsigned long baud = 115200;
constexpr int max_fb_size = 1024;
constexpr int ROWS_PER_BOX = 4;
constexpr int COLS_PER_BOX = 6;
static_assert(ROWS_PER_BOX*COLS_PER_BOX%8 == 0);
char fb[max_fb_size], box_buf[ROWS_PER_BOX*COLS_PER_BOX/8];
int fb_rows = 0, fb_cols = 0;
int row_pos = 0, col_pos = 0;
int row_offset = 0, col_offset = 0;
int row_speed = 0, col_speed = 5;
unsigned int max_fps = 100;
unsigned long start_scrolling_time, last_refresh;
bool scrolling_enabled = false;
bool need_refresh = true;
int max_col_scroll = 0, max_row_scroll = 0;
box_pos box_order[] = {{0, 1, false, false}, {1, 1, false, false}, {2, 1, false, false}, {3, 1, false, false},
                       {3, 0,  true,  true}, {2, 0,  true,  true}, {1, 0,  true,  true}, {0, 0,  true,  true}};

void set_err(const error_type err){
  const char *err_str;
  switch(err){
    case LINE_TO_LONG: err_str = "LINE_TO_LONG"; break;
    case LINE_TO_SHORT: err_str = "LINE_TO_SHORT"; break;
    case BUFFER_FULL: err_str = "BUFFER_FULL"; break;
    case UNKNOWN_SYMBOL: err_str = "UNKNOWN_SYMBOL"; break;
    default: err_str = "UNKNOWN_ERROR";
  }
  node.setProperty("error").send(err_str);
}

bool add_nibble(int &nibbles_in_buf, const char nibble){
    if(nibbles_in_buf/2 == max_fb_size){
      set_err(BUFFER_FULL);
      return false;
    }
    if(nibbles_in_buf%2 == 0){
      fb[nibbles_in_buf/2] = nibble;
      fb[nibbles_in_buf/2] <<= 4;
    }else{
      fb[nibbles_in_buf/2] |= nibble;
    }
    nibbles_in_buf++;
    return true;
}

bool col_pos_handler(const HomieRange &range, const String &value){
  col_pos = value.toInt();
  need_refresh = true;
  return true;
}

bool row_pos_handler(const HomieRange &range, const String &value){
  row_pos = value.toInt();
  need_refresh = true;
  return true;
}

void reset_scrolling(){
  start_scrolling_time = millis();
  need_refresh = true;
  col_offset = 0;
  row_offset = 0;
}

bool frame_handler(const HomieRange &range, const String &value){
  fb_cols = 0;
  fb_rows = 0;
  int nibbles_in_buf = 0, nibbles_in_col = 0;
  for(const char c : value){
    if(c == '\n'){
      if(nibbles_in_col == 0){
        continue;
      }
      if(fb_rows == 0){
        fb_rows = nibbles_in_col*4;
      }else if(nibbles_in_col < fb_rows/4){
        set_err(LINE_TO_SHORT);
        for(; nibbles_in_col < fb_rows/4; nibbles_in_col++){
          if(!add_nibble(nibbles_in_buf, 0)){
            reset_scrolling();
            return true;
          }
        }
      }
      fb_cols++;
      nibbles_in_col = 0;
      continue;
    }
    char nibble;
    if(isdigit(c)){
      nibble = c - '0';
    } else if (c >= 'A' && c <= 'F') {
      nibble = c + 10 - 'A';
    }else{
      set_err(UNKNOWN_SYMBOL);
      continue;
    }
    nibbles_in_col++;
    if(nibbles_in_col*4 > fb_rows && fb_rows != 0){
      set_err(LINE_TO_LONG);
      continue;
    }
    if(!add_nibble(nibbles_in_buf, nibble)){
      reset_scrolling();
      return true;
    }
  }
  reset_scrolling();
  return true;
}

bool scrolling_handler(const HomieRange &range, const String &value){
  scrolling_enabled = value == "true";
  reset_scrolling();
  return true;
}

bool force_refresh_handler(const HomieRange &range, const String &value){
  if(value == "true"){
    need_refresh = true;
    return true;
  }
  return false;
}

bool max_row_scroll_handler(const HomieRange &range, const String &value){
  max_row_scroll = value.toInt();
  reset_scrolling();
  return true;
}

bool max_col_scroll_handler(const HomieRange &range, const String &value){
  max_col_scroll = value.toInt();
  reset_scrolling();
  return true;
}

bool col_speed_handler(const HomieRange &range, const String &value){
  col_speed = value.toInt();
  reset_scrolling();
  return true;
}

bool row_speed_handler(const HomieRange &range, const String &value){
  row_speed = value.toInt();
  reset_scrolling();
  return true;
}

/*
   bit order for every box

    +-------------------+
    | 00 04 08 0C 10 14 |
    | 01 05 09 0D 11 15 |
    | 02 06 0A 0E 12 16 |
    | 03 07 0B 0F 13 17 |
    +-------------------+

     box numberering

      +-----+-----+
      | 0,0 | 1,0 |
      +-----+-----+
      | 0,1 | 1,1 |
      +-----+-----+
  
  note: the bit order for fb is the column wise (the same as for the box)
        and fb_rows is a multiple of 4
*/
void write_frame(){
  for(box_pos &p : box_order){
    memset(box_buf, 0, ROWS_PER_BOX*COLS_PER_BOX/8);
    for(int c = 0; c < COLS_PER_BOX; c++){
      for(int r = 0; r < ROWS_PER_BOX; r++){
        int c_fb = p.col*COLS_PER_BOX + col_pos + col_offset + c;
        int r_fb = p.row*ROWS_PER_BOX + row_pos + row_offset + r;
        if(c_fb >= 0 && r_fb >= 0 && c_fb < fb_cols && r_fb < fb_rows){
          int box_c = c;
          if(p.flip_cols){
            box_c = COLS_PER_BOX - 1 - c;
          }
          int box_r = r;
          if(p.flip_rows){
            box_r = ROWS_PER_BOX - 1 - r;
          }
          int offset_box = box_c*ROWS_PER_BOX + box_r;
          int offset_fb = c_fb*fb_rows + r_fb;
          char val = (fb[offset_fb/8] >> (7 - offset_fb%8)) & 1;
          box_buf[offset_box/8] |= val << (offset_box%8);
        }
      }
    }
    Serial.write(box_buf, ROWS_PER_BOX*COLS_PER_BOX/8);
  }
  Serial.flush();
}

void homie_loop(){
  unsigned long now = millis();
  if(scrolling_enabled){
    int new_row_offset = int(now - start_scrolling_time)*row_speed/1000;
    int new_col_offset = int(now - start_scrolling_time)*col_speed/1000;
    if((max_row_scroll > 0 && max_row_scroll <= abs(new_row_offset)) ||
       (max_col_scroll > 0 && max_col_scroll <= abs(new_col_offset))){
      start_scrolling_time = now;
      new_col_offset = 0;
      new_row_offset = 0;
    }
    need_refresh |= new_row_offset != row_offset;
    need_refresh |= new_col_offset != col_offset;
    row_offset = new_row_offset;
    col_offset = new_col_offset;
  }
  if(need_refresh && now - last_refresh >= 1000u/max_fps){
    write_frame();
    need_refresh = false;
    last_refresh = now;
  }
}
  
void setup(){
  Homie.disableLogging();
  Serial.begin(baud);
  Homie.setLoopFunction(homie_loop);
  Homie_setBrand(BRAND);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  node.advertise("frame").settable(frame_handler);
  node.advertise("force_refresh").settable(force_refresh_handler);
  node.advertise("col-pos").settable(col_pos_handler);
  node.advertise("row-pos").settable(row_pos_handler);
  node.advertise("col-speed").settable(col_speed_handler);
  node.advertise("row-speed").settable(row_speed_handler);
  node.advertise("scrolling").settable(scrolling_handler);
  node.advertise("max-row-scroll").settable(max_row_scroll_handler);
  node.advertise("max-col-scroll").settable(max_col_scroll_handler);
  node.advertise("error");
  Homie.setup();
}

void loop(){
  Homie.loop();
}