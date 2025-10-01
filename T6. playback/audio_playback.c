#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

// --- Función para ajustar volumen ---
/*
void set_volume(long volume) {
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "hw:1"; // Ajusta según tu hardware
    const char *selem_name = "Softmaster";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);

    snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}
*/
// --- Función para reproducir audio WAV ---
void play_audio(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error al abrir archivo WAV");
        exit(1);
    }

    // Saltar cabecera WAV (44 bytes)
    fseek(fp, 44, SEEK_SET);

    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    unsigned int rate = 44100;
    int channels = 2;
    snd_pcm_uframes_t frames = 32;

    snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0);
    snd_pcm_hw_params_set_period_size_near(pcm_handle, params, &frames, 0);
    snd_pcm_hw_params(pcm_handle, params);
    snd_pcm_prepare(pcm_handle);

    int buffer_size = frames * channels * 2; // 16 bits = 2 bytes
    char *buffer = (char *)malloc(buffer_size);

    while (fread(buffer, 1, buffer_size, fp) > 0) {
        snd_pcm_writei(pcm_handle, buffer, frames);
    }

    free(buffer);
    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <archivo.wav> <volumen 0-100>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    long volume = atoi(argv[2]);

    //set_volume(volume);
    play_audio(filename);

    printf("Reproducción terminada.\n");
    return 0;
}
