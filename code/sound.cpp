
struct game_sound_buffer
{
	LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;
	UINT32 RunningSampleIndex = 0;
	int samplesPerSecond;
	int AudioBufferSize;
	int BytesPerSample = 4;
	int SampleLatency = 0;
	int ByteToLock;
	int BytesToWrite;
};

static game_sound_buffer SoundBuffer;

struct game_sound_output_buffer
{
	int SamplePerSecond;
	int SampleCount;
	INT16 *Samples;
};

struct win32_sound_output
{
	int SamplesPerSecond;
	int ToneHz;
	int ToneVolume;
	int WavePeriod;
	int BytesPerSample;
	int SecondaryBufferSize;
	int LatencySampleCount;
};


typedef struct {
	uc16 ChunkID[4];
	ui32 ChunkSize;
	ui32 Format;
	uc16 Subchunk1ID[4];
	ui32 Subchunk1Size;
	ui16 AudioFormat;
	ui16 NumChannels;
	ui32 SampleRate;
	ui32 ByteRate;
	ui16 BlockAlign;
	ui16 BytesPerSample;
	uc16 Subchunk2ID[4];
	ui16 Subchunk2Size;
} WAVEHEADER;

typedef struct {
	ui32 frameSync : 11;
	byte mpegID : 2;
	byte layerDesc : 2;
	byte protection : 1;
	byte bitRateIndex : 4;
	byte sampleRate : 2;
	byte paddingBit : 1;
	byte privateBit : 1;
	byte channelMode : 2;
} MP3HEADER;

/*
internal void
DEBUGDrawSoundCursors(game_offscreen_buffer *Buffer, game_sound_buffer *SoundBuffer)
{
 DWORD PlayCursor;
 DWORD WriteCursor;
 
 SoundBuffer->GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor);
 
 int padding = 16;
 float wRatio = ((float)Buffer->BitmapWidth - (2.0f * (float)padding)) / (float)SoundBuffer->AudioBufferSize;
 int writeX = wRatio * (float)WriteCursor;
 int playX = wRatio * (float)PlayCursor;
 int h = Buffer->BitmapHeight - padding;
 
 Line(Buffer, playX, padding, playX, h, 255, 255, 255);
 Line(Buffer, writeX, padding, writeX, h, 0, 255, 0);
}
*/
void
init_direct_sound(HWND Window, INT32 SamplesPerSecond, game_sound_buffer *SoundBuffer)
{
    HMODULE DSoundLibrary = LoadLibrary("dsound.dll");
    direct_sound_create *DirectSoundCreate = (direct_sound_create *)GetProcAddress(DSoundLibrary, "DirectSoundCreate");
    if(DSoundLibrary)
    {
        LPDIRECTSOUND DirectSound;
        if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
        {
            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nSamplesPerSec = SamplesPerSecond;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nChannels = 2;
            WaveFormat.nBlockAlign = (WaveFormat.wBitsPerSample / 8)  * WaveFormat.nChannels;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
            WaveFormat.cbSize = 0;
            HRESULT Error = DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY);
            if (SUCCEEDED(Error))
            {
                DSBUFFERDESC BufferDescription = {};
                BufferDescription.dwSize = sizeof(BufferDescription);
                BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER || DSBCAPS_GETCURRENTPOSITION2;
                
                LPDIRECTSOUNDBUFFER PrimaryBuffer;
                if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
                {
                    if (SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat)))
                    {
						
                    }
                    else
                    {
                        
                    }
                }
                else
                {
                    
                }
            }
            else
            {
                
            }
            DSBUFFERDESC BufferDescription = {};
            BufferDescription.dwSize = sizeof(BufferDescription);
            BufferDescription.dwFlags = 0;
            BufferDescription.dwBufferBytes = SoundBuffer->AudioBufferSize;
            BufferDescription.lpwfxFormat = &WaveFormat;
            HRESULT Error2 = DirectSound->CreateSoundBuffer(&BufferDescription, &SoundBuffer->GlobalSecondaryBuffer, 0);
            if (SUCCEEDED(Error2))
            {
            }
        }
    }
}
/*
static bool open_wav_file(char *FileName, game_sound_buffer *SoundBuffer)
{
 FILE *File;
 WAVEHEADER WaveHeader = {};
 errno = 0;
 if (fopen_s(&File, FileName, "r") == 0) {
  int count = fread(&WaveHeader, sizeof(WaveHeader), 1, File); 
  if (count == 1)
  {
   if (WaveHeader.AudioFormat == 1)
   {
 LPVOID AudioPortion1 = 0;
 LPVOID AudioPortion2 = 0;
 
 DWORD AudioPortion1Size = 0;
 DWORD AudioPortion2Size = 0;
 fseek(File, 0L, SEEK_END);
 int BytesToRead = ftell(File) - sizeof(WAVEHEADER);
 int errr = SoundBuffer->GlobalSecondaryBuffer->Lock(0, BytesToRead, &AudioPortion1, &AudioPortion1Size, &AudioPortion2, &AudioPortion2Size, DSBLOCK_ENTIREBUFFER);
 if (!AudioPortion1) return false;
 if (!AudioPortion2 && AudioPortion2Size > 0)
 {
 
 }
 
 fseek(File, sizeof(WAVEHEADER), SEEK_SET);
 
 char* sound_data = (char *)malloc(BytesToRead);
 int read_bytes = fread(sound_data, 1, BytesToRead, File);
 
 memcpy(AudioPortion1, sound_data, BytesToRead);
 
 SoundBuffer->GlobalSecondaryBuffer->Unlock(AudioPortion1, AudioPortion1Size, AudioPortion2, AudioPortion2Size);
 fclose(File);
 free(sound_data);
 
 int err = errno;
 cout << errno << endl;
 
   }
  }
  else
  {
  
   //Error with count.
  }
 }
 else
 {
  //Could not load the file.
  
 }
}
*/

struct wav_file_data
{
	char* filename;
	char *storage;
	int size_in_bytes;
};

static bool direct_sound_load_wav(wav_file_data *data, game_sound_buffer *sound_buffer)
{
	
	LPVOID AudioPortion1 = 0;
	LPVOID AudioPortion2 = 0;
	DWORD AudioPortion1Size = 0;
	DWORD AudioPortion2Size = 0;
	HRESULT error;
	error = sound_buffer->GlobalSecondaryBuffer->Lock(0, data->size_in_bytes, &AudioPortion1, &AudioPortion1Size, &AudioPortion2, &AudioPortion2Size, DSBLOCK_ENTIREBUFFER);
	if (!AudioPortion1) return false;
	if (!AudioPortion2 && AudioPortion2Size > 0)
	{
		
	}
	memcpy(AudioPortion1, data->storage, data->size_in_bytes);
	//NOTE: For some odd reason Unlock function MUST be passed with original LPVOID AudioPortion1/2 pointers, you have to memcpy data to it.
	AudioPortion1Size = data->size_in_bytes;
	error  = sound_buffer->GlobalSecondaryBuffer->Unlock(AudioPortion1, AudioPortion1Size, AudioPortion2, AudioPortion2Size);
}

static bool open_wav_file(char *filename, wav_file_data *data_out)
{
	FILE *file;
	WAVEHEADER wave_header = {};
	errno = 0;
	if (fopen_s(&file, filename, "r") == 0) {
		int count = fread(&wave_header, sizeof(wave_header), 1, file); 
		if (count == 1)
		{
			if (wave_header.AudioFormat == 1)
			{
				fseek(file, 0L, SEEK_END);
				int bytes_to_read = ftell(file) - sizeof(WAVEHEADER);
				data_out->storage = (char*)malloc(bytes_to_read);
				fseek(file, sizeof(wave_header), SEEK_SET);
				fread(data_out->storage, 1, bytes_to_read, file);
				data_out->filename = filename;
				data_out->size_in_bytes = bytes_to_read;
				return 1;
			}
		}
		else
		{
		}
	}
	else
	{
		cout << "open_wav_file: error loading file." << endl;
		return 0;
	}
}



/*
static bool open_mp3_file(const char *FileName, game_sound_buffer *SoundBuffer)
{
 FILE *File;
 MP3HEADER mp3Header = {};
 errno = 0;
 if (fopen_s(&File, FileName, "rb") == 0) {
  if (int Count = fread(&mp3Header, sizeof(MP3HEADER), 1, File) == 1)
  {
   if (mp3Header.frameSync == 0xff){
 if (mp3Header.mpegID ^ 10000000)
 {
 
 }
   }
   else
   {
 //Wrong audio format.
   }
   
  }
  else
  {
   LPVOID AudioPortion1 = 0;
   LPVOID AudioPortion2 = 0;
   
   DWORD AudioPortion1Size = 0;
   DWORD AudioPortion2Size = 0;
   
   int BytesToRead = 4096;
   
   int errr = SoundBuffer->GlobalSecondaryBuffer->Lock(0, BytesToRead, &AudioPortion1, &AudioPortion1Size, 0, 0, DSBLOCK_ENTIREBUFFER);
   
   if (!AudioPortion1) return false;
   
   fseek(File, sizeof(WAVEHEADER), SEEK_SET);
   
   char* TempWaveData = (char *)malloc(AudioPortion1Size);
   int Count2 = fread(TempWaveData, 1, BytesToRead, File);
   
   memcpy(AudioPortion1, TempWaveData, BytesToRead);
   
   SoundBuffer->GlobalSecondaryBuffer->Unlock(AudioPortion1, AudioPortion1Size, AudioPortion2, AudioPortion2Size);
   fclose(File);
   
   
   int err = errno;
   
   
   //Error with count.
  }
 }
 else
 {
  //Could not load the file.
  
 }
}
*/