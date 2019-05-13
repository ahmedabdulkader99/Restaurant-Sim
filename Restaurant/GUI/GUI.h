#ifndef __GUI_H_
#define __GUI_H_

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\Defs.h"

#include "..\Rest\Order.h"

#include <string>
#include "../Rest/Motorcycle.h"
using namespace std;

class GUI
{

private:
	window *pWind;
	color OrdersClrs[TYPE_CNT];
	PROG_MODE P;

	struct servMoto {
		int ID;
		int speed;
		int distance;
		int xPos;
		int yPos;
		ORD_TYPE type;
		int increment;
		bool serviced = 0;
		bool finished = 0;
	};

	int activeMCount;

	servMoto* motoOnRoad[100];

	// Some Static Constant Data Members: ---------------------

	static const int	
		WindWidth=1200, WindHeight=650,		//Window width and height
		StatusBarHeight = 150,	//Status Bar Height
		MenuBarHeight = 50,		//Menu Bar Height (distance from top of window to bottom line of menu bar)
		MenuItemWidth = 80,		//Width of each item in menu bar menu
		
		DrawingAreaHeight = WindHeight - MenuBarHeight - StatusBarHeight,		
		YHalfDrawingArea = MenuBarHeight + DrawingAreaHeight/2, //The y coordinate of half the Drawing area

		RestStartX = (int)(WindWidth * 0.4), //The x coordinate of the upper left corner of the Rest
		RestEndX = (int)(WindWidth * 0.6),   //The x coordinate of the lower right corner of the Rest

		RestWidth = RestEndX - RestStartX, //The width of the Rest (Note: the Rest is a Square)		

		RestStartY = YHalfDrawingArea - RestWidth/2, //The y coordinate of the upper left corner of the Rest
		RestEndY = YHalfDrawingArea + RestWidth/2,   //The y coordinate of the lower right corner of the Rest


		FontSize = 20,	//font size used to draw orders ID on Interface
		OrderWidth = 2*FontSize,	//width of the order to be drawn on GUI
		OrderHeight = FontSize,		//height of the order to be drawn on GUI


		MaxHorizOrders	= ((WindWidth-RestWidth)/2 ) / (OrderWidth+1),		//The max no. of orders the can be drwan in on Horizontal line in a region
		MaxVerticalOrders = (DrawingAreaHeight /2)/(OrderHeight + 1),   //The max no. of orders the can be drwan in on Horizontal line in a region


		//Max no of orders that can be drawn in a single region
		MaxRegionOrderCount  = MaxHorizOrders*MaxVerticalOrders;
		
		
		

	/////////
	////////
	////////

	int OrderCount;		//the total number of orders to be drawn
	
	Order * OrdListForDrawing[MaxPossibleOrdCnt]; // This Array of Pointers is used for drawing elements in the GUI
	//NOTES: 
	//Orders are assumed to be sorted by arrival time
	// At every time step, you should update those pointers 
	// to point to the current waiting orders only
	
	string oCnt[4];
	string mCnt[4];




	// 
	// TODO: Add more members if needed
	//

	void DrawSingleOrder(Order* pO, int RegionCount) const;		//draws ONE order 
	void DrawOrders() const;		//drwas ALL orders in OrdListForDrawing

	void DrawMotoOnRoad(servMoto* M);
	void DrawAllMotos();

	void DrawCounts() const;

	void DrawString(const int iX, const int iY, const string Text); // prints a message in the passed coordinates
	void DrawRestArea() const;	    // draws the restaurant area
	void DrawMotoRoad();

	void ClearStatusBar() const;    // clears the status bar
	void ClearDrawingArea() const;	// clears the Drawing area from all drawings

public:
	GUI();
	~GUI();

	// Input Functions  ---------------------------
	void waitForClick() const;	// waits a user click from the user
	string GetString() const ; // reads a string (keyboard input) from the user

	// Output Functions  ---------------------------
	void PrintMessage(string msg1, string msg2 = " ") const; // prints a message in the status bar
	void PrintState();

	void UpdateDrawnCounts(int* N, int* F, int* V, int* NM, int* FM, int* VM);
	void UpdateInterface();
	void AddOrderForDrawing(Order* pOrd);	//Adds a new order to the drawing list
	void RemoveOrderFromDrawing(int id); //Removes Order from drawing list
	void ResetDrawingList();		//resets drawing list (should be called every timestep after drawing)

	void AddMotoToRoad(Motorcycle* pMoto);

	PROG_MODE getGUIMode();			//returns the mode of the program

};

#endif