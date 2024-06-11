#include "splashkit.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <queue>
#include "unisex.cpp"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>


bool start_simulation = false;
bool isValidInput = true;
window main_window;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
font font1 = load_font("font1", "./font/PressStart2P-Regular.ttf");
float speed = 1.5;
string current_number = "";
std::vector<int> peopleInToilet;

struct button
{
    rectangle rect;
    string label;
};

bool isPersonInQueue(std::vector<int> &waitingQueue, int id) {
    return std::find(waitingQueue.begin(), waitingQueue.end(), id) != waitingQueue.end();
}

std::vector<std::string> read_actions_from_file(const std::string& filename) {
    std::vector<std::string> actions;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            actions.push_back(line);
        }
    }
    return actions;
}

void run_unisex(string number) {
    HANDLE h[THREADCOUNT];
	DWORD ThreadId;
    maxPeople = std::stoi(number);
	srand(time(0));
	f.open("output.txt",std::ios::out);
	
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	empty = CreateSemaphore(NULL, 1, 1, NULL);
	turnstile = CreateSemaphore(NULL, 1, 1, NULL);
	male = CreateSemaphore(NULL, 1, 1, NULL);
	female = CreateSemaphore(NULL, 1, 1, NULL);
	maleCount = CreateSemaphore(NULL, maxPeople, maxPeople, NULL);
	femaleCount = CreateSemaphore(NULL, maxPeople, maxPeople, NULL);
	
	
	for(int i = 0; i < THREADCOUNT ; i++){
		int k = rand()%2;
		if(k == 1){
			h[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MalesProcess, NULL, 0, &ThreadId);
		}
		else{
			h[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FemalesProcess, NULL, 0, &ThreadId);
		}
	}
	
	WaitForMultipleObjects(THREADCOUNT, h, TRUE, INFINITE);
	f.close();
}

void draw_person(string gender, string id, double x, double y, double radius)
{
    color color;
    if (gender == "Girl") {
        color = COLOR_PINK;
    }
    else {
        color = COLOR_BLUE;
    }
    fill_circle(color, x, y - 3.5 * radius, radius);
    fill_circle(color, x, y, radius * 2);
    fill_rectangle(COLOR_WHITE, x - 2 * radius, y, radius * 4.5, radius * 2.5);
    draw_text(id, COLOR_BLACK, x - 5, y + 5);
}

vector<button> create_numpad()
{
    vector<button> numpad;
    int start_x = 400, start_y = 200;
    int width = 50, height = 50, gap = 10;

    for (int i = 0; i < 10; i++)
    {
        button btn;
        btn.rect = rectangle_from(start_x + (i % 3) * (width + gap),
                                  start_y + (i / 3) * (height + gap), 
                                  width, height);
        btn.label = std::to_string(i);
        numpad.push_back(btn);
    }

    return numpad;
}

button create_delete_button()
{
    return {rectangle_from(460, 380, 110, 50), "Delete"};
}

button create_start_button()
{
    return {rectangle_from(400, 500, 170, 50), "Start Simulation"};
}

void draw_button(const button &btn)
{
    draw_rectangle(COLOR_GRAY, btn.rect);
    draw_text(btn.label, COLOR_BLACK, "font1", 20, 
              btn.rect.x + btn.rect.width / 2 - 10, 
              btn.rect.y + btn.rect.height / 2 - 10);
}


void run_simulation(const string& number_to_display)
{
    run_unisex(number_to_display);
    std::vector<std::string> actions = read_actions_from_file("output.txt");
    std::vector<std::string> split_actions;
    
    std::string act;
    for (auto action:actions) {
        std::stringstream ss(action);
        while (std::getline(ss, act, ',')) {
            if (!act.empty() && !std::all_of(act.begin(), act.end(), isspace)) {
                split_actions.push_back(act);
            }
        }
    }
    
    auto it = split_actions.begin();
    int counter = 0;
    std::vector<int> waitingQueue;
    while (!quit_requested() || it != split_actions.end() )
    {
        if (key_typed(ESCAPE_KEY)) {
            close_window(main_window);
        }
        const auto& action = *it;
        process_events();
        clear_screen(COLOR_WHITE);

        draw_text("The Unisex Bathroom Problem Simulation: " + number_to_display, COLOR_BLACK, "font1", 20, 100, 10); 
        draw_text(action, COLOR_BLACK, "font1", 15, 10, WINDOW_HEIGHT - 30); 
        std::istringstream ss(action);
        std::string word;
        std::vector<std::string> words;
        while (ss >> word) {
            words.push_back(word);
        }
        counter++;



        int rect_width = 100;
        int rect_height = 70;
        int rect_gap = 5;
        int rect_x = WINDOW_WIDTH - rect_width - rect_gap - 200;
        int rect_y = 50;
        int num_toilet = std::stoi(number_to_display);
        rectangle bathroom_rect = rectangle_from(rect_x - 50, rect_y - 10, 350, 500);
        draw_rectangle(COLOR_GRAY, bathroom_rect);
        draw_text("People in toilet: " + std::to_string(peopleInToilet.size()), COLOR_BLACK, "font1", 15, bathroom_rect.x + bathroom_rect.width / 2 - 150, bathroom_rect.y + 5);
        if (std::stoi(number_to_display) >8) {
            num_toilet = 8;
            draw_text(".........................", COLOR_BLACK, "font1", 10, bathroom_rect.x + 50, WINDOW_HEIGHT - 100);
        }
        for (int i = 0; i < num_toilet; i++)
        {
            if (i % 2 == 0) {
                rectangle toilet_rect = rectangle_from(rect_x, rect_y + (i * (rect_height + rect_gap - 20) + 20), rect_width, rect_height);
                draw_rectangle(COLOR_GRAY, toilet_rect);
                draw_text("toilet", COLOR_BLACK, toilet_rect.x + toilet_rect.width / 2 - 20, toilet_rect.y + toilet_rect.height / 2);
            }
            else {
                rectangle toilet_rect = rectangle_from(rect_x + 150, rect_y + ((i - 1) * (rect_height + rect_gap - 20) + 20), rect_width, rect_height);
                draw_rectangle(COLOR_GRAY, toilet_rect);
                draw_text("toilet", COLOR_BLACK, toilet_rect.x + toilet_rect.width / 2 - 20, toilet_rect.y + toilet_rect.height / 2);
            }
        }


        rectangle waiting_room = rectangle_from(WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT / 2 - 60, 60, 60);
        draw_rectangle(COLOR_GRAY, waiting_room);
        draw_text("Waiting room", COLOR_BLACK, waiting_room.x - 10, waiting_room.y - 20);
        draw_text("People waiting: " + std::to_string(waitingQueue.size()), COLOR_BLACK, waiting_room.x - 30, waiting_room.y + waiting_room.height + 30);

        rectangle room = rectangle_from(20, rect_y - 10, 420, 500);
        int person_width = 50;
        int person_height = 60;
        int people_per_row = room.width / person_width;
        int people_per_column = room.height / person_height;
        std::srand(static_cast<unsigned int>(time(0)));
        draw_rectangle(COLOR_GRAY, room);
        for (int i = 0; i < 64; i++) {
            int row = i / people_per_row;
            int col = i % people_per_row;
            int x = room.x + col * person_width + 30;
            int y = room.y + row * person_height + 50;
            draw_person((i % 2 == 0) ? "Boy" : "Girl", "", x, y, 10);
        }


        if (words[3] == "in") {
            if (!isPersonInQueue(peopleInToilet, std::stoi(words[1]))) {
                peopleInToilet.push_back(std::stoi(words[1]));
            }
            if (isPersonInQueue(waitingQueue, std::stoi(words[1])))
            {
                if (waiting_room.x + counter * speed - 20 < rect_x - 60) {
                    draw_person(words[0], words[1], waiting_room.x + counter * speed, waiting_room.y + 60, 10);
                }
                else {
                    waitingQueue.erase(std::remove(waitingQueue.begin(), waitingQueue.end(), std::stoi(words[1])), waitingQueue.end());
                    counter = 150;
                }
            }
            else {
                if (460 + counter * speed + 15 < rect_x - 60) {
                    draw_person(words[0], words[1], 460 + counter * speed, 150, 10);
                }
            }
        }
        else if (words[3] == "out")
        {
            peopleInToilet.erase(std::remove(peopleInToilet.begin(), peopleInToilet.end(), std::stoi(words[1])), peopleInToilet.end());
            if (rect_x - 50 - counter * speed - 20 > 460) {
                draw_person(words[0], words[1], rect_x - 50 - counter * speed - 20, 500, 10);
            }
        }
        else {
            if (!isPersonInQueue(waitingQueue, std::stoi(words[1])))
            {
                waitingQueue.push_back(std::stoi(words[1]));
            }
            if (460 + counter * speed - 20 < waiting_room.x) {
                draw_person(words[0], words[1], 460 + counter * speed, 200 + counter * speed, 10);
            }
        }

        if (counter == 180) {
            counter = 0;
            ++it;
        }
        refresh_screen(60);
    }
}




void handle_mouse_click(const vector<button>& numpad, const button& start_btn, const button& delete_btn)
{
    point_2d mouse = mouse_position();

    if (mouse_down(LEFT_BUTTON))
    {
        for (const button &btn : numpad)
        {
            if (point_in_rectangle(mouse, btn.rect))
            {
                current_number += btn.label;
                delay(200);
            }
        }

        if (point_in_rectangle(mouse, start_btn.rect))
        {
            if (current_number == "0" || current_number == "") {
                isValidInput = false;
            }
            else {
                run_simulation(current_number);
            }
        }

        if (point_in_rectangle(mouse, delete_btn.rect))
        {
            if (!current_number.empty())
            {
                current_number.pop_back();
                delay(200);
            }
        }
    }
}

int main()
{
    main_window = open_window("Unisex Bathroom Simulation", WINDOW_WIDTH, WINDOW_HEIGHT);
    vector<button> numpad = create_numpad();
    button start_btn = create_start_button();
    button delete_btn = create_delete_button();
    while (!quit_requested())
    {
        process_events();
        handle_mouse_click(numpad, start_btn, delete_btn);

        clear_screen(COLOR_WHITE);
        for (const button &btn : numpad)
            draw_button(btn);
        draw_rectangle(COLOR_GRAY, start_btn.rect);
        draw_rectangle(COLOR_GRAY, delete_btn.rect);
        draw_text("Start simulation", COLOR_BLACK, "font1", 10, start_btn.rect.x + 5, start_btn.rect.y + start_btn.rect.height / 2 - 5);
        draw_text("Delete", COLOR_BLACK, "font1", 10, 485, 400);
        draw_text("The Unisex Bathroom Problem Simulation", COLOR_BLACK, "font1", 25, 30, 100);
        draw_text("Enter The Max People Number: " + current_number, COLOR_BLACK, "font1", 20, 200, 150, option_to_screen());
        if (!isValidInput) {
            draw_text("Invalid input. Please enter a valid number", COLOR_RED, "font1", 10, 250, 180, option_to_screen());
        }
        
        refresh_screen(60);
    }
    return 0;
}
