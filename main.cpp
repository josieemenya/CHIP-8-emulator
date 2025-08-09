#define SDL_MAIN_HANDLED
#include <iostream>
#include "chip8.h"
#include "platform_layer.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.


void mainloop(bool* quit, Platform platform, Chip8 chip8, auto lastCycleTime, int cycleDelay){
        while (!quit)
    {
        quit = platform.ProcessInput(chip8.keypad);
        //std::cout << "After ProcessInput, quit=" << quit << std::endl;

        if (quit)
            std::cerr << "Quit Signal Recieved\n";

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
        //std::cout << "dt = " << dt << ", cycleDelay = " << cycleDelay << std::endl;


        if (dt > cycleDelay)
        {
            lastCycleTime = currentTime;
            
            chip8.Cycle();

            if (!chip8.display) {
                throw std::runtime_error("Display Returned Null!");
            }


            //std::cout << "Calling Update()" << std::endl;
            std::cout.flush(); 
            platform.Update(chip8.display, videoPitch);
            //std::cout << "Update() Complete" << std::endl;
        }
    }
}



int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cerr << "nxan"; 

   

    int videoScale = (argc > 1) ? atoi(argv[1]) : 10;
    int cycleDelay = (argc > 2) ? atoi(argv[2]) : 4;
    
    char const* romFilename = (argc > 3 && argv[3] != nullptr) ? argv[3] : "../ROMS/test_opcode.ch8";

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError();
    std::cerr.flush();

    return 1;
    }

    Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

    Chip8 chip8;
    chip8.loadProgram(romFilename);



    int videoPitch = sizeof(chip8.display[0]) * VIDEO_WIDTH;
    std::cout << videoPitch << std::endl; 

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
    #else
    while (1) { mainloop(&quit, platform, chip8, lastCycleTime, cycleDelay); }
    #endif




    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.