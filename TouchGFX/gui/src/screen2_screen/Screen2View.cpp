#include <gui/screen2_screen/Screen2View.hpp>
#include "cmsis_os.h"

extern osMessageQueueId_t Queue1Handle;

static uint32_t randomSeed = 1474521;
static uint32_t simpleRandom() {
	randomSeed = randomSeed * 1103515245 + 12345;
	return (randomSeed / 65536) % 795369439;
}

Screen2View::Screen2View()
{
	tickCount = 0;
	speed = 1;
	finish = false;
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();

    gameOver.setVisible(false);
    win.setVisible(false);
    enemyBullet.setVisible(false);
    playerBullet.setVisible(false);
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}
