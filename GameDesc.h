#pragma once

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

#define WINDOW_TITLE L"BoxShooter"

#define ENTITY_MAX_COUNT 1000

//地图和像素大小比例
#define MAP_RATIO 0.01f

/*
* 将从0开始的ENTITY_CONSTANT_MAX个id作为固定id分配给固定的entity，比如player
* 从ENTITY_CONSTANT_MAX开始的ENTITY动态分配
*/
#define ENTITY_CONSTANT_COUNT 100

#define ENTITY_ID_PLAYER 1
#define ENTITY_ID_CURSOR 2

