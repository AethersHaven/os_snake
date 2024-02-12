void main()
{
    char* video_memory = 0xA0000;
    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 200; y++)
        {
            video_memory[y * 320 + x] = 0x2F;
        }
    }
}