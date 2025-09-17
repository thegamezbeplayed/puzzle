#include <stdio.h>
#include "game_utils.h"
#include "raylib.h"
#include "game_assets.h"

static audio_manager_t AudioMan;

void InitAudio(){
  AudioMan = (audio_manager_t){0};
  char fullpath[256]; // make sure it's large enough

  //FilePathList audiofiles = LoadDirectoryFiles("resources/sfx/action");
  for(int i = 0; i < SFX_DONE; i++){
    AudioMan.sfx[i] = (sfx_group_t){0};
    AudioMan.timers[i] = InitEvents();
  }

  for (int i = 0; i < END_SFX; i++){
    sfx_info_t info = sfx_catalog[i];
    AudioMan.sfx[info.group].num_types++;

    AudioMan.sfx[info.group].items[i] =(sfx_sound_t) {0};
    AudioMan.sfx[info.group].items[i].type =info.type;
    AudioMan.sfx[info.group].items[i].num_sounds = info.num_sounds;

    AudioMan.sfx[info.group].items[i].sounds = calloc(info.num_sounds,sizeof(Sound));
    for (int j = 0; j < info.num_sounds; j++){
      sprintf(fullpath, "resources/sfx/%s", info.paths[j]);
      AudioMan.sfx[info.group].items[i].sounds[j] = LoadSound(fullpath);

    TraceLog(LOG_INFO,"Found sfx file %s",fullpath);

    }

  }
}

void AudioPlayRandomSfx(SfxGroup group, SfxType type){
  if(CheckEvent(AudioMan.timers[group],EVENT_PLAY_SFX))
    return;

  for(int i = 0; i < AudioMan.sfx[group].num_types; i++){
    if(AudioMan.sfx[group].items[i].type != type)
      continue;

    int r = rand()%AudioMan.sfx[group].items[i].num_sounds;
    PlaySound(AudioMan.sfx[group].items[i].sounds[r]);

    int wait  = (int)((AudioMan.sfx[group].items[i].sounds[r].frameCount*30)/44100);
    AddEvent(AudioMan.timers[group],InitCooldown(wait,EVENT_PLAY_SFX,NULL,NULL));
    return;
  }
}

void AudioStep(){
  for(int g = 0; g<SFX_DONE;g++){
    StepEvents(AudioMan.timers[g]);
  }

}
