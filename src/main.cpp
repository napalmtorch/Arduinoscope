#include <Arduino.h>

char buff[32];
char input[384];
int inpos;

bool pins[256];

void reset_input();

void setup()
{
    inpos = 0;
    memset(buff, 0, sizeof(buff));
    memset(pins, 0, sizeof(pins));
    memset(input, 0, sizeof(input));

    Serial.begin(9600);
}

void loop() 
{
    for (int i = 0; i < sizeof(pins); i++)
    {
        if (pins[i])
        {
            
            Serial.write(">");
            
            memset(buff, 0, sizeof(buff));
            itoa(i, buff, 10);
            Serial.write(buff);
            Serial.write(":");

            int val = (i >= 54 ? analogRead(i) : digitalRead(i));
            memset(buff, 0, sizeof(buff));
            itoa(val, buff, 10);
            Serial.write(buff);

            Serial.write("\n");
        }
    }


    if (Serial.available() > 0)
    {
        if (inpos < sizeof(input)) { input[inpos++] = Serial.read(); }

        if (!strcmp(input, "-C\n"))
        {
            memset(pins, 0, sizeof(pins));
            reset_input();
        }
        else if (input[0] == '+' || input[0] == '-')
        {
            if (input[1] == 'A' || input[1] == 'D' || input[1] == 'd')
            {
                char temp[16];
                strcpy(temp, input + 2);
                if (temp[strlen(temp) - 1] == '\n')
                {
                    int num = atoi(temp);
                    pins[num] = (input[0] == '+' ? true : false);
                    if (input[1] == 'D' || input[1] == 'A') { pinMode(num, (pins[num] == true ? INPUT : OUTPUT)); }
                    else { pinMode(num, (pins[num] == true ? INPUT_PULLUP : OUTPUT)); }
                    reset_input();
                }
            }
        }
    }
}

void reset_input()
{
    memset(input, 0, sizeof(input));
    inpos = 0;
}