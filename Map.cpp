#include "Map.h"
#include "Texture_Handler.h"

Map::Map(){

	//All Textures to be loaded here

}

void Map::Load(int arr, int x, int y) {
	
	int type= 0;
	
	for (int r = 0; r < x; r++) {

		for (int c = 0; c < y; y++) {

			type = map[r][c];
			
			switch (type) {

			case 0:
				break;
			}
		}
	}
}

void Map::Draw() {

}