#include "GUI.h"

//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	pWind = new window(WindWidth+15,WindHeight,0,0); 
	pWind->ChangeTitle("The Restautant");

	OrderCount = 0;

	//Set color for each order type
	OrdersClrs[TYPE_NRM] =  FLATDARKBLUE;	//normal-order color
	OrdersClrs[TYPE_FROZ] = FLATLIGHTBLUE;		//Frozen-order color
	OrdersClrs[TYPE_VIP] = 	FLATRED;		//VIP-order color					

	ClearStatusBar();
	ClearDrawingArea(); 
	DrawRestArea();  
	
}
//////////////////////////////////////////////////////////////////////////////////////////
GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x,y;
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}
//////////////////////////////////////////////////////////////////////////////////////////
string GUI::GetString() const 
{
	string Label;
	char Key;
	while(1)
	{
		pWind->WaitKeyPress(Key);
		if(Key == 27 )	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if(Key == 13 )	//ENTER key is pressed
			return Label;
		if((Key == 8) && (Label.size() >= 1))	//BackSpace is pressed
			Label.resize(Label.size() -1 );			
		else
			Label += Key;
		
		PrintMessage(Label);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg1, string msg2) const	//Prints a message on status bar
{
	ClearStatusBar();	//First clear the status bar
	
	pWind->SetPen(WHITE);
	pWind->SetFont(18, BOLD , BY_NAME, "Arial");   
	pWind->DrawString(10, WindHeight - (int) (StatusBarHeight/2), msg2);
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.5), msg1);// You may need to change these coordinates later 
	                                                                      // to be able to write multi-line
}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawString(const int iX, const int iY, const string Text)
{
	pWind->SetPen(WHITE);
	pWind->SetFont(18, BOLD , BY_NAME, "Arial");   
	pWind->DrawString(iX, iY, Text);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GUI::ClearStatusBar() const
{
	pWind->SetPen(FLATDARKBLUE, 3);
	pWind->SetBrush(FLATDARKBLUE);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight , WindWidth, WindHeight);	

	pWind->SetPen(FLATDARKBLUE, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight , WindWidth, WindHeight - StatusBarHeight);	
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::ClearDrawingArea() const
{
	// Clearing the Drawing area
	pWind->SetPen(FLATDARKBLUE, 3);
	pWind->SetBrush(FLATDARKBLUE);
	pWind->DrawRectangle(0, 0, WindWidth, WindHeight - StatusBarHeight);
	pWind->SetPen(FLATWHITE, 3);
	pWind->SetBrush(FLATWHITE);
	pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::DrawRestArea() const
{
	int L = RestWidth / 2;

	// 1- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(FLATDARKBLUE, 3);
	pWind->DrawLine(0, YHalfDrawingArea, WindWidth, YHalfDrawingArea);
	pWind->DrawLine(WindWidth/2, MenuBarHeight, WindWidth/2, WindHeight-StatusBarHeight);

	// 2- Drawing the brown square of the Rest
	pWind->SetPen(FLATRED);
	pWind->SetBrush(FLATRED);
	pWind->DrawRectangle(RestStartX, RestStartY, RestEndX, RestEndY);

	// 3- Drawing the 2 white crossed lines (inside the Rest)
	pWind->SetPen(DARKRED, 2);
	pWind->DrawLine(WindWidth/2, YHalfDrawingArea - RestWidth/2, WindWidth/2, YHalfDrawingArea + RestWidth/2);
	pWind->DrawLine(WindWidth/2 - RestWidth/2, YHalfDrawingArea, WindWidth/2 + RestWidth/2, YHalfDrawingArea);

	// 4- Drawing the 4 white squares inside the Rest (one for each tower)
	/*pWind->SetPen(FLATLIGHTBLUE);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(RestStartX + L/3, RestStartY + L/3, RestStartX + 2*L/3, RestStartY + 2*L/3);
	pWind->DrawRectangle(RestStartX + L/3, RestEndY - L/3, RestStartX + 2*L/3, RestEndY - 2*L/3);
	pWind->DrawRectangle(RestEndX - 2*L/3, RestStartY + L/3, RestEndX - L/3, RestStartY + 2*L/3);
	pWind->DrawRectangle(RestEndX - 2*L/3, RestEndY - L/3, RestEndX - L/3, RestEndY - 2*L/3);*/

	// 5- Writing the letter of each region (A, B, C, D)
	pWind->SetPen(FLATWHITE);
	pWind->SetFont(60, BOLD , BY_NAME, "Arial");
	pWind->DrawString(RestStartX + (int)(0.36*L), RestStartY + 3*L/12, "A");
	pWind->DrawString(RestStartX + (int)(0.36*L), YHalfDrawingArea + 3*L/12, "D");
	pWind->DrawString(WindWidth/2 + (int)(0.34*L), RestStartY + 3*L/12, "B");
	pWind->DrawString(WindWidth/2 + (int)(0.34*L), YHalfDrawingArea + 3*L/12, "C"); 

}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawSingleOrder(Order* pO, int RegionCount) const       // It is a private function
{

	if (RegionCount > MaxRegionOrderCount) 
		return; //no more orders can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if(RegionCount>=MaxHorizOrders )	//max no. of orders to draw in one line
	{
		DrawDistance = (RegionCount-1)%MaxHorizOrders + 1;
		YPos = (RegionCount-1) / MaxHorizOrders + 1; 
	}

	color clr = OrdersClrs[pO->GetType()];
	REGION Region = pO->GetRegion();

	int x, y, refX, refY;
	//First calculate x,y position of the order on the output screen
	//It depends on the region and the order distance
	switch (Region)
	{
	case A_REG:
		refX = (WindWidth/2 - RestWidth/2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX - DrawDistance*OrderWidth - DrawDistance; //(Distance)
		y = refY - YPos*OrderHeight - YPos; // YPos
		break;
	case B_REG:
		refX = (WindWidth/2 + RestWidth/2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX + (DrawDistance-1)*OrderWidth + DrawDistance; //(Distance)
		y = refY - YPos*OrderHeight - YPos; // YPos
		break;
	case C_REG:
		refX = (WindWidth/2 + RestWidth/2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX + (DrawDistance-1)*OrderWidth + DrawDistance; //(Distance)
		y = refY + (YPos-1)*OrderHeight + YPos; // YPos
		break;
	case D_REG:
		refX = (WindWidth/2 - RestWidth/2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX - DrawDistance*OrderWidth - DrawDistance; //(Distance)
		y = refY + (YPos-1)*OrderHeight + YPos; // YPos
		break;
	default:
		break;
	}

	// Drawing the Order
	pWind->SetPen(clr);
	pWind->SetBrush(clr);
	//pWind->DrawRectangle(x, y, x + OrderWidth, y + OrderHeight);
	pWind->SetFont(20,BOLD, MODERN);
	pWind->DrawInteger(x,y,pO->GetID());
}


//////////////////////////////////////////////////////////////////////////////////////////
/* A function to draw a list of orders and ensure there is no overflow in the drawing*/
// To use this function, you must prepare its input parameters as specified
// [Input Parameters]:
//    orders [ ] : array of Order pointers (ALL orders from all regions in one array)
//    TotalOrders : the size of the array (total no. of orders)
void GUI::DrawOrders() const
{

	//Prepare counter for each region
	int RegionsCounts[REG_CNT]={0};	//initlaize all counters to zero

	for(int i=0; i<OrderCount; i++)
	{
		int orderRegion = OrdListForDrawing[i]->GetRegion();
		RegionsCounts[orderRegion]++;
		DrawSingleOrder(OrdListForDrawing[i], RegionsCounts[orderRegion]);
	}

}

void GUI::DrawCounts() const
{
	int L = RestWidth / 2;
	pWind->SetPen(WHITE);
	pWind->SetFont(20, BOLD, BY_NAME, "Arial");
	pWind->DrawString(RestStartX + (int)(0.09 * L), RestStartY + 1 * L / 12, oCnt[0]);
	pWind->DrawString(RestStartX + (int)(0.09 * L), YHalfDrawingArea + 9 * L / 12, oCnt[3]);
	pWind->DrawString(WindWidth / 2 + (int)(0.09 * L), RestStartY + 1 * L / 12, oCnt[1]);
	pWind->DrawString(WindWidth / 2 + (int)(0.09 * L), YHalfDrawingArea + 9 * L / 12, oCnt[2]);

	pWind->SetPen(WHITE);
	pWind->SetFont(15, BOLD, BY_NAME, "Arial");
	pWind->DrawString(RestStartX + (int)(0.1 * L), RestStartY + 9 * L / 12, mCnt[0]);
	pWind->DrawString(RestStartX + (int)(0.1 * L), YHalfDrawingArea + 1 * L / 12, mCnt[3]);
	pWind->DrawString(WindWidth / 2 + (int)(0.1 * L), RestStartY + 9 * L / 12, mCnt[1]);
	pWind->DrawString(WindWidth / 2 + (int)(0.1 * L), YHalfDrawingArea + 1 * L / 12, mCnt[2]);
}

void GUI::UpdateDrawnCounts(int* N, int* F, int* V, int* NM, int* FM, int* VM)
{
	for (int i = 0; i < 4; i++) {
		string sN;
		string sF;
		string sV;
		sN = to_string(N[i]);
		sF = to_string(F[i]);
		sV = to_string(V[i]);
		oCnt[i] = "N: " + sN + " F: " + sF + " V: " + sV;
		sN = to_string(NM[i]);
		sF = to_string(FM[i]);
		sV = to_string(VM[i]);
		mCnt[i] = "Nor: " + sN + " Frz: " + sF + " Fst: " + sV;
	}

}

void GUI::UpdateInterface()
{
	ClearDrawingArea();
	DrawRestArea();
	DrawCounts();
	DrawOrders();
}

/*
	AddOrderForDrawing: Adds a new order to the drawing list
*/
void GUI::AddOrderForDrawing(Order* ptr)
{
	if (OrderCount < MaxPossibleOrdCnt) 
		OrdListForDrawing[OrderCount++] = ptr;

	// Note that this function doesn't allocate any Order objects
	// It only makes the first free pointer in the array
	// points to the same Order pointed to by "ptr"
}

void GUI::RemoveOrderFromDrawing(int id)
{
	for (int i = 0; i < OrderCount; i++) {
		if (OrdListForDrawing[i]->GetID() == id) {
			OrderCount--;
			for (int j = i; j < OrderCount+1; j++) {
				OrdListForDrawing[j] = OrdListForDrawing[j + 1];
			}
			break;
		}
	}
	return;
}


void GUI::ResetDrawingList()
{
	OrderCount = 0;		//resets the orders count to be ready for next timestep updates
}


PROG_MODE	GUI::getGUIMode() const
{
	PROG_MODE Mode;
	do
	{
		PrintMessage("Please select GUI mode: (1)Interactive, (2)StepByStep, (3)Silent... ");
		string S = GetString();
		Mode = (PROG_MODE) (atoi(S.c_str())-1);
	}
	while(Mode< 0 || Mode >= MODE_CNT);
	
	return Mode;
}
