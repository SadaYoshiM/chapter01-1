#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir1(0)
,mPaddleDir2(0)
{

}


bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: $s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter 01)",
		100,	//左上隅のx座標
		100,	//左上隅のy座標
		1024,	//幅
		768,	//高さ
		0	//フラグ
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,	//レンダラの描画対象となるウィンドウ
		-1,	//通常-1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddlePos1.x = 10.0f;
	mPaddlePos1.y = 768.0f / 2.0f;
	mPaddlePos2.x = 1024.0f - 10.0f - thickness;
	mPaddlePos2.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleDir1 = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir1 -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir1 += 1;
	}
	mPaddleDir2 = 0;
	if (state[SDL_SCANCODE_I]) {
		mPaddleDir2 -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mPaddleDir2 += 1;
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) {
		;
	}

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	//player1
	if (mPaddleDir1 != 0) {
		mPaddlePos1.y += mPaddleDir1 * 300.0f * deltaTime;
		if (mPaddlePos1.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos1.y = paddleH / 2.0f + thickness;
		}else if (mPaddlePos1.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddlePos1.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	//player2
	if (mPaddleDir2 != 0) {
		mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
		if (mPaddlePos2.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos2.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddlePos2.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1;
	}
	else if (mBallPos.y >= 768.0f - thickness && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	}

	float diff1 = mPaddlePos1.y - mBallPos.y;
	diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
	float diff2 = mPaddlePos2.y - mBallPos.y;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if (diff1 <= paddleH / 2.0f && mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && mBallVel.x < 0.0f) {
		mBallVel.x *= -1;
	}else if (diff2 <= paddleH / 2.0f && mBallPos.x >= 1024.0f - 25.0f && mBallPos.x <= 1024.0f - 20.0f && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}
	else if (mBallPos.x <= 0.0f || mBallPos.x >= 1024.0f) {
		mIsRunning = false;
	}
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(
		mRenderer,
		0,	//Red
		0,	//Green
		255,	//Blue
		255	//Alpha
	);
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);
	SDL_Rect wall{
		0,	//x
		0,	//y
		1024,	//w
		thickness	//h
	};
	SDL_RenderFillRect(mRenderer, &wall);

	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos1.x),
		static_cast<int>(mPaddlePos1.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	paddle.x = static_cast<int>(mPaddlePos2.x);
	paddle.y = static_cast<int>(mPaddlePos2.y - paddleH / 2);
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);
}