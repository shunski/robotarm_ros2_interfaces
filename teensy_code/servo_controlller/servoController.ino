#include <IcsHardSerialClass.h>
#include <TimeLib.h>
const byte EN_PIN1 = 2;
const long BAUDRATE = 115200;
const int TIMEOUT = 1000;
IcsHardSerialClass krs1(&Serial1, EN_PIN1, BAUDRATE, TIMEOUT);


static int count_servo = 0;
unsigned int dt = 1000000;

class KServo
{
    private:
      unsigned int id;
      unsigned int goal_pos;
      unsigned int ideal_pos;
      unsigned int home_pos;
      unsigned int num_steps;
      int step_value;
      const int max_sp = 1/95.6668;//position per microsecon
    
    public:
      KServo(){
        Serial.println("Constructor of KServo");
        id = count_servo;
        count_servo++;
        home_pos = 7500;
        num_steps = 0;
      }
      
      
      bool set_work(float g_pos, int micro_t){
        Serial.println("Setwork of KServo");
          ideal_pos = krs1.getPos(id);
          goal_pos = g_pos;
          Serial.print("goal_pos:  ");
          Serial.println(goal_pos);
          Serial.print("ideal_pos:  ");
          Serial.println(ideal_pos);
          
          int cur_sp = (goal_pos - ideal_pos) / micro_t;
          if(goal_pos < 3500 || goal_pos > 11500)
            return false;
          else if(cur_sp < max_sp)
            return false;
          else {
            num_steps = micro_t / dt;  
            step_value = (goal_pos - ideal_pos) / num_steps;
            Serial.print("num_steps = ");
            Serial.print(num_steps);
            Serial.print("step_value = ");
            Serial.println(step_value);
          }
          return true;
      }
     
      void init(){
        Serial.println("init of KServo");
        //set this current position to the home position 
        set_work(home_pos, 1.0);
      }
      
      void update(){
        Serial.println("----------Update of KServo-----------");
        Serial.print("num_step");
        Serial.println(num_steps);
        if (num_steps > 0){
          Serial.print(ideal_pos);
          ideal_pos += step_value;
          Serial.print(" now moves to ");
          Serial.println(ideal_pos);
          krs1.setPos(id, ideal_pos);
          num_steps--;
        }
        Serial.println("----------Updated -----------");
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
          Serial.println("Constructor of Servo Controller");
          servo_ptr = new KServo[num_servos];
      }
      
      void init(){
        Serial.println("init of Servo Controller");
        last_update_t = micros();
        for(unsigned int i=0; i<num_servos; i++)
          servo_ptr[i].init();
      }
    
      bool set_work(unsigned int id, float angle, unsigned int sec_t){
        Serial.println("Setwork of Servo Controller");
        unsigned int micro_t = sec_t * 1000000;
        float goal_pos  = krs1.degPos(angle); 
        if(servo_ptr[id].set_work(goal_pos, micro_t))
            return true;
        else
            return false;
      }
    
      void update(){
        Serial.println("Update of Servo Controller");
        unsigned long current_t = micros();
        Serial.print("current_t - last_update_t = ");
        Serial.print(current_t - last_update_t);
        Serial.print("   dt: ");
        Serial.println(dt);
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

  delay(1000);
  id = (id + 1) % 2;
  if(controller.set_work(id, 90.0, 1))//angle, second
     Serial.println("===========Set Work Success=========");
  else
     Serial.println("Set Work Fail");
  delay(5000);
}
