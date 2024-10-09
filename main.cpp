#include <SDL.h>
#include <iostream>
#include <tuple>
#include <chrono>

using namespace std;

const int width = 800;
const int height = 800;
const double G = 2;	//controls simulation speed

int render_mode = 2;		//Simulation = 1, Fractal=2

int maxIterations = pow(10, 6);		//  /!\ IF YOU CHANGE DIMENSIONS; ADJUST THIS /!\

struct planet{
	double mass;
	double pos_x;
	double pos_y;
	double v_x;
	double v_y;
	double a_x;
	double a_y;
	bool fixed;
};

planet p1 = { 100, 0, 0, 0.001, 0.001, 0, 0, false };
planet p2 = { 300, 400, 400, 0, 0, 0, 0, false };
planet p3 = { 100, 10, 400, 0, 0, 0, 0, false };

struct vector {
	double intensity;
	double angle;
	int x_sign;	
	int y_sign;
};





std::tuple<planet&, planet&, planet&> parameter_updater(planet &p1,planet &p2,planet &p3){
	double dtime = 3;
	double dtime2 = pow(dtime, 2);

	
	double p12_distance_x = p1.pos_x - p2.pos_x;
	double p12_distance_y = p1.pos_y - p2.pos_y;
	double angle = 0;
	if (p12_distance_x != 0) {angle = abs(atan(p12_distance_y / p12_distance_x));}
	vector p12_distance = { sqrt(pow(p12_distance_x,2) + pow(p12_distance_y,2)),angle, 1, 1 };
	if (p12_distance_x < 0) { p12_distance.x_sign *= -1; }
	if (p12_distance_x == 0) { p12_distance.x_sign = 0; }
	if (p12_distance_y < 0) { p12_distance.y_sign *= -1; }
	if (p12_distance_y == 0) { p12_distance.y_sign = 0; }
	vector attraction_p12 = { G * p1.mass * p2.mass / pow(p12_distance.intensity,2), p12_distance.angle, p12_distance.x_sign, p12_distance.y_sign };

	double p13_distance_x = p1.pos_x - p3.pos_x;
	double p13_distance_y = p1.pos_y - p3.pos_y;
	angle = 0;
	if (p13_distance_x != 0) { angle = abs(atan(p13_distance_y / p13_distance_x)); }
	vector p13_distance = { sqrt(pow(p13_distance_x,2) + pow(p13_distance_y,2)),angle, 1, 1 };
	if (p13_distance_x < 0) { p13_distance.x_sign *= -1; }
	if (p13_distance_x == 0) { p13_distance.x_sign = 0; }
	if (p13_distance_y < 0) { p13_distance.y_sign *= -1; }
	if (p13_distance_y == 0) { p13_distance.y_sign = 0; }
	vector attraction_p13 = { G * p1.mass * p3.mass / pow(p13_distance.intensity,2), p13_distance.angle, p13_distance.x_sign, p13_distance.y_sign };

	double p23_distance_x = p3.pos_x - p2.pos_x;
	double p23_distance_y = p3.pos_y - p2.pos_y;
	angle = 0;
	if (p23_distance_x != 0) { angle = abs(atan(p23_distance_y / p23_distance_x)); }
	vector p23_distance = { sqrt(pow(p23_distance_x,2) + pow(p23_distance_y,2)),angle, 1, 1 };
	if (p23_distance_x < 0) { p23_distance.x_sign *= -1; }
	if (p23_distance_x == 0) { p23_distance.x_sign = 0; }
	if (p23_distance_y < 0) { p23_distance.y_sign *= -1; }
	if (p23_distance_y == 0) { p23_distance.y_sign = 0; }
	vector attraction_p23 = { G * p2.mass * p3.mass / pow(p23_distance.intensity,2), p23_distance.angle, p23_distance.x_sign, p23_distance.y_sign };

	if (not p1.fixed) {
		double p1_newPos_x = 0.5 * p1.a_x * dtime2 + p1.v_x * dtime + p1.pos_x;
		double p1_newPos_y = 0.5 * p1.a_y * dtime2 + p1.v_y * dtime + p1.pos_y;
		double p1_newV_x = p1.a_x * dtime + p1.v_x;
		double p1_newV_y = p1.a_y * dtime + p1.v_y;
		double p1_force_x = -attraction_p12.x_sign * attraction_p12.intensity * cos(attraction_p12.angle) - attraction_p13.x_sign * attraction_p13.intensity * cos(attraction_p13.angle);
		double p1_force_y = -attraction_p12.y_sign * attraction_p12.intensity * sin(attraction_p12.angle) - attraction_p13.y_sign * attraction_p13.intensity * sin(attraction_p13.angle);
		double p1_newA_x = p1_force_x / p1.mass;
		double p1_newA_y = p1_force_y / p1.mass;
		p1 = { p1.mass, p1_newPos_x, p1_newPos_y, p1_newV_x, p1_newV_y, p1_newA_x, p1_newA_y };
	}

	if (not p2.fixed) {
		double p2_newPos_x = 0.5 * p2.a_x * dtime2 + p2.v_x * dtime + p2.pos_x;
		double p2_newPos_y = 0.5 * p2.a_y * dtime2 + p2.v_y * dtime + p2.pos_y;
		double p2_newV_x = p2.a_x * dtime + p2.v_x;
		double p2_newV_y = p2.a_y * dtime + p2.v_y;
		double p2_force_x = +attraction_p12.x_sign * attraction_p12.intensity * cos(attraction_p12.angle) + attraction_p23.x_sign * attraction_p23.intensity * cos(attraction_p23.angle);
		double p2_force_y = +attraction_p12.y_sign * attraction_p12.intensity * sin(attraction_p12.angle) + attraction_p23.y_sign * attraction_p23.intensity * sin(attraction_p23.angle);
		double p2_newA_x = p2_force_x / p2.mass;
		double p2_newA_y = p2_force_y / p2.mass;
		p2 = { p2.mass, p2_newPos_x, p2_newPos_y, p2_newV_x, p2_newV_y, p2_newA_x, p2_newA_y };
	}

	if (not p3.fixed) {
		double p3_newPos_x = 0.5 * p3.a_x * dtime2 + p3.v_x * dtime + p3.pos_x;
		double p3_newPos_y = 0.5 * p3.a_y * dtime2 + p3.v_y * dtime + p3.pos_y;
		double p3_newV_x = p3.a_x * dtime + p3.v_x;
		double p3_newV_y = p3.a_y * dtime + p3.v_y;
		double p3_force_x = attraction_p13.x_sign * attraction_p13.intensity * cos(attraction_p13.angle) - attraction_p23.x_sign * attraction_p23.intensity * cos(attraction_p23.angle);
		double p3_force_y = attraction_p13.y_sign * attraction_p13.intensity * sin(attraction_p13.angle) - attraction_p23.y_sign * attraction_p23.intensity * sin(attraction_p23.angle);
		double p3_newA_x = p3_force_x / p3.mass;
		double p3_newA_y = p3_force_y / p3.mass;
		p3 = { p3.mass, p3_newPos_x, p3_newPos_y, p3_newV_x, p3_newV_y, p3_newA_x, p3_newA_y };
	}
	

	return std::tie(p1,p2,p3);
}



int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	SDL_RenderSetLogicalSize(renderer, width, height);

	if (window == NULL) {
		std::cout << "Window could not be opened: " << SDL_GetError() << std::endl;
	}




	if (render_mode == 1) {
		while (true) {
			if (SDL_PollEvent(&event) and event.type == SDL_QUIT) {
				break;
			}
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawPoint(renderer, p1.pos_x, p1.pos_y);
			SDL_RenderDrawPoint(renderer, p2.pos_x, p2.pos_y);
			SDL_RenderDrawPoint(renderer, p3.pos_x, p3.pos_y);
			SDL_RenderPresent(renderer);

			std::tie(p1, p2, p3) = parameter_updater(p1, p2, p3);
		}
	}

	if (render_mode == 2){
		auto t = chrono::system_clock::now().time_since_epoch();
		auto start_time = chrono::duration_cast<chrono::milliseconds>(t).count();
		std::cout << start_time;

		int i = 0;
		int max_i = 10;
		int index = 0;
		int percentage = 0;
		planet p2_t;
		planet p3_t;

		while (true) {
		bool not_completed = true;
		if (SDL_PollEvent(&event) and event.type == SDL_QUIT) {
			break;
		}
		if (not_completed){
			for (int x =0; x <= width; x++) {
				
				for (int y = 0; y <= height; y++) {
					p1.pos_x = x;
					p1.pos_y = y;

					i = 0;
					while (p1.pos_x <= width and p1.pos_x >= 0 and p1.pos_y <= height and p1.pos_y >= 0 and i < maxIterations) {
						std::tie(p1, p2_t, p3_t) = parameter_updater(p1, p2, p3);
						i++;
					}

					if (i > max_i and i!=maxIterations) {
						max_i = i;
					}

					int color = 510*i / max_i;
					int blue = 0;
					int red = 0;
					if (color > 255) {
						blue = 255;
						red = color - 255;
					}
					else { blue = color; red = 0; }
					SDL_SetRenderDrawColor(renderer, red, 5, blue, 255);
					SDL_RenderDrawPoint(renderer, x, y);

					if (round((index) / static_cast<float>(width * height) * 100)> percentage) {
						auto t_2 = chrono::system_clock::now().time_since_epoch();
						auto current_time = chrono::duration_cast<chrono::milliseconds>(t_2).count();
						std::cout << "\r" << ++percentage << "% done " << ((current_time - start_time) * (100 - percentage)) / ((percentage) * 1000) << " seconds left. " << max_i << std::flush;
					}
					index++;
				}
			}
			SDL_RenderPresent(renderer);
		}
		not_completed = false;
		}
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	return 0;

}