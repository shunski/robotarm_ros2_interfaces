#include <IcsHardSerialClass.h>
#include <TimeLib.h>

const byte EN_PIN1 = 2;
const long BAUDRATE = 115200;
const int TIMEOUT = 1000;

IcsHardSerialClass krs1(&Serial1, EN_PIN1, BAUDRATE, TIMEOUT);


static int count_servo = 0;
unsigned int dt = 10000;

class KServo
{
    private:
      unsigned int id;
      int goal_pos;
      int ideal_pos;
      int home_pos;
      int num_steps;
      int step_value;
      const int max_spd = (60.0/0.17) * (11500-3500)/270; //position per sec
    
    public:
      KServo(){
        id = count_servo;
        count_servo++;
        home_pos = 7500;
        num_steps = 0;
        ideal_pos = 0;
      }
      
      
      bool set_work(int g_pos, unsigned int micro_t){
          this->ideal_pos = krs1.getPos(id);
          this->goal_pos = g_pos;

          int spd = 1.0 * (this->goal_pos - this->ideal_pos) / (micro_t / 1000000.0);
          spd = abs(spd);
          
          if( this->goal_pos < 3500 || this->goal_pos > 11500 ){
            Serial.println("pos invalid");
            return false;
          }
          else if( spd <= 0 || spd > max_spd ){
            Serial.println("speed invalid");
            return false;
          }
          else {
            this->num_steps = micro_t / dt;  
            this->step_value = (goal_pos - ideal_pos) / num_steps;
          }
          return true;
      }
     
      void init(){
        //set this current position to the home position 
        this->set_work(this->home_pos, 1000000);
      }
      
      void update(){
        if (num_steps > 0){
          this->ideal_pos += this->step_value;
          krs1.setPos(this->id, this->ideal_pos);
          num_steps--;
        }
      }


};


class ServoController
{ 
    private:
      unsigned long last_update_t;
      unsigned int num_servos;
      const unsigned int dt = 1000000;
      KServo* servo_ptr;
      
    public: 
      
      ServoController(unsigned int num_servos, unsigned int dt): 
        num_servos(num_servos), dt(dt)
      {
          servo_ptr = new KServo[num_servos];
      }
      
      void init(){
        Serial.println("init of Servo Controller");
        last_update_t = micros();
        for(unsigned int i=0; i<num_servos; i++)
          servo_ptr[i].init();
      }
    
      bool set_work(unsigned int id, float angle, float sec_t){
        unsigned int micro_t = sec_t * 1000000;
        int goal_pos  = krs1.degPos(angle); 
        if(servo_ptr[id].set_work(goal_pos, micro_t))
            return true;
        else
            return false;
      }
    
      void update(){
        unsigned long current_t = micros();
        if( current_t - last_update_t > dt){
          for (unsigned int i=0; i<num_servos; i++)
            servo_ptr[i].update();
          last_update_t = current_t;
        }
      }
};








int id=0;

ServoController controller(2, dt); // 2 = { # of servos }, dt[microsec]

void setup() {
  krs1.begin();
  Serial.begin(115200);
  delay(1000);
  controller.init();
  
}

void loop() {
  controller.update();
  
  if( Serial.available() ){
    float angle = Serial.parseFloat();
    Serial.print("moving to: ");
    Serial.println(angle);
    controller.set_work(id, angle, 0.5);
    id = (id + 1) % 2;
  }
  
  delay(1);
}
