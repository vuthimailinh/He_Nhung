#include <images/BitmapDatabase.hpp>
#include <gui/screen2_screen/Screen2View.hpp>

#include "cmsis_os.h"

extern osMessageQueueId_t inputQueueHandle;
extern osMessageQueueId_t audioQueueHandle;
uint8_t queue_result = 0;

void shootAudio(){
	 uint8_t data = 'D';
	osMessageQueuePut(audioQueueHandle, &data, 0, 10);
}
static uint32_t randomSeed = 1474521;
static uint32_t simpleRandom() {
	randomSeed = randomSeed * 1103515245 + 12345;
	return (randomSeed / 65536) % 3;
}

bool checkCollision (const touchgfx::Image& img1, const touchgfx::Image& img2) {
	return (img1.getX() + img1.getWidth()	> img2.getX() &&
			img1.getX() - img2.getWidth()	< img2.getX() &&
			img1.getY() + img1.getHeight() 	> img2.getY() &&
			img1.getY() - img2.getHeight()	< img2.getY() );
}


int mbi = 0;
int ebi = 0;
int bbi = 0;
constexpr int maxBullet = 10;


float enemy1XVel = -2;
float enemy2XVel = 1.4142;
float bossXVel = 2.7828;

int enemy1HP = 0;
int enemy2HP = 0;
int bossHP = 0;

static int highScore = 0;


uint16_t enemy1Score = 0;
uint16_t enemy2Score = 0;
uint16_t bossScore = 0;


touchgfx::Image Mbullet[maxBullet];

touchgfx::Image Ebullet[maxBullet];

touchgfx::Image Bbullet[maxBullet];

Screen2View::Screen2View()
{
	tickCount = 0;
	speed = 2;
	finish = false;
}


void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();

    for( int i = 0; i < maxBullet; i++){
		Mbullet[i].setBitmap(touchgfx::Bitmap(BITMAP_PLAYERBULLET_ID));
		Mbullet[i].setY(-100);
		add(Mbullet[i]);

		Ebullet[i].setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_SWIPECONTAINER_SMALL_ON_ACTIVE_ID));
		Ebullet[i].setY(400);
		add(Ebullet[i]);

		Bbullet[i].setBitmap(touchgfx::Bitmap(BITMAP_BULLET_ID));
		Bbullet[i].setY(400);
		add(Bbullet[i]);
    }

    boss.setXY(20, 20);
    enemy1.setXY(200, 60);
    enemy2.setXY(100, 90);



    gameOver.setVisible(false);
    win.setVisible(false);
    enemyBullet.setVisible(false);
    playerBullet.setVisible(false);
    enemyBullet_1.setVisible(false);
    bossBullet.setVisible(false);
    homeButton.setVisible(false);
    textScore.setVisible(false);
    textHighScore.setVisible(false);

    boss.setVisible(true);
    enemy1.setVisible(true);
    enemy2.setVisible(true);
    playerShip.setVisible(true);

    finish = false;
    score = 0;

    enemy1HP = 1;
    enemy2HP = 1;
    bossHP = 1;

    enemy1Score = 1000;
    enemy2Score = 2000;
    bossScore = 5000;
}

void Screen2View::handleTickEvent() {
	Screen2ViewBase::handleTickEvent();

	tickCount ++;

	if (finish) return;

	if (osMessageQueueGetCount(inputQueueHandle) > 0) {
		osMessageQueueGet(inputQueueHandle, &queue_result, NULL, osWaitForever);
		switch (queue_result) {
		case 'L': //left
			if (playerShip.getX() - speed > 0) {
				playerShip.setX(playerShip.getX() - speed);
			} else {
				playerShip.setX(0);
			}
			break;
		case 'R': //right
			if (playerShip.getX() + speed < 240 - playerShip.getWidth()) {
				playerShip.setX(playerShip.getX() + speed);
			} else {
				playerShip.setX(240 - playerShip.getWidth());
			}
			break;
		case 'S': //shoot
			Mbullet[mbi].setXY(playerShip.getX() + playerShip.getWidth()/2 - Mbullet[mbi].getWidth()/2, playerShip.getY());
			Mbullet[mbi].setVisible(true);
			mbi = (mbi + 1) % maxBullet;
			break;
		default:
			break;
		}
	}


	if (enemy1.getX() + enemy1XVel < 0 || enemy1.getX() + enemy1XVel > 240 - enemy1.getWidth()) {
		enemy1XVel = -enemy1XVel;
	} else {
		enemy1.setX(enemy1.getX() + enemy1XVel);
	}

	if (enemy2.getX() + enemy2XVel < 0 || enemy2.getX() + enemy2XVel > 240 - enemy2.getWidth()) {
		enemy2XVel = -enemy2XVel;
	} else {
		enemy2.setX(enemy2.getX() + enemy2XVel);
	}

	if (boss.getX() + bossXVel < 0 || boss.getX() + bossXVel > 240 - boss.getWidth()) {
		bossXVel = -bossXVel;
	} else {
		boss.setX(boss.getX() + bossXVel);
	}

	if (tickCount % 200 == 0) {
		uint32_t randomNumber = simpleRandom();
		switch (randomNumber) {
		case 0:
			if (enemy1HP > 0) {
				Ebullet[ebi].setXY(enemy1.getX(), enemy1.getY());
				Ebullet[ebi].setVisible(true);
				ebi = (ebi + 1) % maxBullet;
			}
			break;
		case 1:
			if (enemy2HP > 0) {
				Ebullet[ebi].setXY(enemy2.getX(), enemy2.getY());
				Ebullet[ebi].setVisible(true);
				ebi = (ebi + 1) % maxBullet;
			}
			break;
		case 2:
			if (bossHP > 0) {
				Bbullet[bbi].setXY(boss.getX(), boss.getY());
				Bbullet[bbi].setVisible(true);
				bbi = (bbi + 1) % maxBullet;
			}
			break;
		default:
			break;
		}
	}

	if (tickCount % 5000 == 0) {
		enemy1Score = uint16_t (enemy1Score * 0.9);
		enemy2Score = uint16_t (enemy2Score * 0.9);
		bossScore = uint16_t (bossScore * 0.9);
	}


	for (int i = 0; i < maxBullet; i++) {
		if (Ebullet[i].getY() + 3 < 400) {
			Ebullet[i].setY(Ebullet[i].getY() + 3);
		}
		if (Bbullet[i].getY() + 3 < 400) {
			Bbullet[i].setY(Bbullet[i].getY() + 3);
		}

		if (checkCollision(Ebullet[i], playerShip) || checkCollision(Bbullet[i], playerShip)) {
			if (checkCollision(Ebullet[i], playerShip)) {
				Ebullet[i].setY(400);
			} else {
				Bbullet[i].setY(400);
			}

			finish = true;
			playerShip.setVisible(false);
			gameOver.setVisible(true);
		    homeButton.setVisible(true);
		    Unicode::snprintf(textScoreBuffer, 6, "%d", (int)score);
			Unicode::snprintf(textHighScoreBuffer, 6, "%d", (int)highScore);
		    textScore.setVisible(true);

		    textHighScore.setVisible(true);

		    invalidate();
			return;
		}
	}

	for (int i = 0; i < maxBullet; i++) {
		if (Mbullet[i].getY() - 3 > -100) {
			Mbullet[i].setY(Mbullet[i].getY() - 3);
		}

		if (checkCollision(Mbullet[i], enemy1)) {
			Mbullet[i].setY(-100);
			shootAudio();
			enemy1HP--;
			if (enemy1HP <= 0) {
				enemy1.setY(400);
				score += enemy1Score;
			}
		}
		if (checkCollision(Mbullet[i], enemy2)) {
			Mbullet[i].setY(-100);
			enemy2HP--;
			shootAudio();
			if (enemy2HP <= 0) {
				enemy2.setY(400);
				score += enemy2Score;
			}
		}
		if (checkCollision(Mbullet[i], boss)) {
			Mbullet[i].setY(-100);
			shootAudio();
			bossHP--;
			if (bossHP <= 0) {
				boss.setY(400);
				score += bossScore;
			}
		}
	}

	if (score > highScore) {
		highScore = score;
	}

	if (enemy1HP <= 0 && enemy2HP <= 0 && bossHP <= 0) {
		finish = true;
		win.setVisible(true);
	    homeButton.setVisible(true);

	    Unicode::snprintf(textScoreBuffer, 6, "%d", (int)score);
		Unicode::snprintf(textHighScoreBuffer, 6, "%d", (int)highScore);
		textScore.setVisible(true);
		textHighScore.setVisible(true);

	    invalidate();

	}

	invalidate();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}
