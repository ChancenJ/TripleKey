#include "app_audio.h"


#include "AudioFileSourceFunction.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include <AudioFileSourceLittleFS.h>

 



void app_audio_poweron()
{
    pinMode(PIN_AUDIO_SD, OUTPUT);
    digitalWrite(PIN_AUDIO_SD, HIGH); //开启I2S

}

void app_audio_poweroff()
{
    pinMode(PIN_AUDIO_SD, OUTPUT);
    digitalWrite(PIN_AUDIO_SD, LOW); //关闭I2S
}


void app_audio_play_wav(const char *path)
{
    AudioGeneratorWAV *wav;
    AudioFileSourceLittleFS *file;
    AudioOutputI2S *out;
    Serial.println(path);

    app_audio_poweron();
    out = new AudioOutputI2S();https://hackaday.io/projects
    
        out->SetPinout(PIN_AUDIO_BCK, PIN_AUDIO_WS, PIN_AUDIO_DATA);
    file = new AudioFileSourceLittleFS(path);
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);
    while (1)
    {
        if (wav->isRunning())
        {
            if (!wav->loop())
            {
                uint32_t e = millis();
                wav->stop();
            }
        }
        else
        {
            break;
        }
    }
    app_audio_poweroff();
    delete (file);
    file = nullptr;
    delete (wav);
    wav = nullptr;
    delete (out);
    out = nullptr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    pinMode(BUTTON3_PIN,INPUT);//boot引脚与i2s之间存在的奇怪问题，若使用i2s，则io0无法使用了,因此需要重新配置为输入
    

 
}



const char *spHour[] = {"12", "1", "2", "3", "4", "5", "6",
                        "7", "8", "9", "10", "11"};

const char *spMinDec[] = {"0", "10", "20", "30", "40", "50"};

const char *spMinSpecial[] = {"11", "12", "13", "14",
                              "15", "16", "17", "18",
                              "19"};
const char *spMinLow[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
void app_audio_sayTimeCN(int hour, int minutes)
{
#if defined( SUPPORT_AUDIO)
    bool pm = (hour >= 12);

    app_audio_play_wav("/cn_girl/the time is.wav");
    if (pm)
    {
        app_audio_play_wav("/cn_girl/afternoon.wav");
    }
    else
    {
        app_audio_play_wav("/cn_girl/morning.wav");
    }

    hour = hour % 12;
    char buffer[20];
    sprintf(buffer, "/cn_girl/%s.wav", spHour[hour]);
    app_audio_play_wav(buffer);
    app_audio_play_wav("/cn_girl/dian.wav");
    if (minutes == 0)
    {
        app_audio_play_wav("/cn_girl/zheng.wav");
        // talkie->say(spOCLOCK, sizeof(spOCLOCK));
    }
    else if (minutes <= 10 || minutes >= 20)
    {
        memset(buffer, 0, 20);
        // talkie->say(spMinDec[minutes / 10], spMinDecLen[minutes / 10]);
        sprintf(buffer, "/cn_girl/%s.wav", spMinDec[minutes / 10]);
        app_audio_play_wav(buffer);
        if (minutes % 10)
        {
            memset(buffer, 0, 20);
            sprintf(buffer, "/cn_girl/%s.wav", spMinLow[(minutes % 10) - 1]);
            app_audio_play_wav(buffer);
            // talkie->say(spMinLow[(minutes % 10) - 1], spMinLowLen[(minutes % 10) - 1]);
        }
        app_audio_play_wav("/cn_girl/fen.wav");
    }
    else
    {
        memset(buffer, 0, 20);
        sprintf(buffer, "/cn_girl/%s.wav", spMinSpecial[minutes - 11]);
        app_audio_play_wav(buffer);
        // talkie->say(spMinSpecial[minutes - 11], spMinSpecialLen[minutes - 11]);
        app_audio_play_wav("/cn_girl/fen.wav");
    }

#endif
    
}