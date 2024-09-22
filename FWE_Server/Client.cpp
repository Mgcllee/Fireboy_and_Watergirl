#pragma once

#include "Client.h"

Client::Client() 
	: role(false)
{
}

Client::~Client() {}

void Client::set_ready_for_play(int user_ticket)
{
	S2CPlayerPacket loadPacket;
	loadPacket.type = S2CLoading;
	loadPacket.id = user_ticket;
	clientId = user_ticket;
	threadHandle = CreateThread(NULL, 0, ClientWorkThread, reinterpret_cast<LPVOID>(i), 0, NULL);
	
	multiEvenTthreadHadle[i] = threadHandle;
	
	send(socket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);

	loadPacket.type = S2CAddPlayer;
	for (int j = 0; j < 3; j++) {
		if (i != j) {
			if (threadHandles[j].threadHandle != NULL) {
				send(threadHandles[j].clientSocket, (char*)&loadPacket, sizeof(S2CPlayerPacket), 0);

				S2CPlayerPacket addPlayerPacket;
				addPlayerPacket.type = S2CAddPlayer;
				addPlayerPacket.id = j;
				send(clientSocket, (char*)&addPlayerPacket, sizeof(S2CPlayerPacket), 0);
			}
		}
	}

	if (i == 2) {
		S2CChangeStagePacket changePacket;
		changePacket.stageNum = STAGE_ROLE;
		changePacket.type = S2CChangeStage;

		for (int x = 0; x < 3; x++) {
			send(threadHandles[x].clientSocket, (char*)&changePacket, sizeof(S2CChangeStagePacket), 0);
		}
		stageIndex = STAGE_ROLE;
	}
}

void Client::run_client_thread()
{
	while (socket != INVALID_SOCKET) {
		int recvRetVal = recv(socket, recvBuf + prevSize, MAX_BUF_SIZE - prevSize, 0);
		if (recvRetVal > 0) {
			ConstructPacket(threadHandles[myIndex], recvRetVal);
		}

		if (StageMgr.currentVisibleJewely.OBJECT_Collide(threadHandles[myIndex])) {
			S2CPlayerPacket jewelyPacket;
			jewelyPacket.id = -1;
			jewelyPacket.type = S2CEatJewely;
			DWORD retVal = WaitForSingleObject(jewelyEatHandle, 0);
			if (retVal != WAIT_OBJECT_0) {
				threadHandles[myIndex].score++;
				SetEvent(jewelyEatHandle);
				jewelyPacket.id = myIndex;
			}
			if (jewelyPacket.id != -1)
				for (int j = 0; j < 3; j++)
					send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&jewelyPacket), sizeof(S2CPlayerPacket), 0);

		}

		if (isVisibleDoor) {
			if (StageMgr.door.OBJECT_Collide(threadHandles[myIndex])) {
				DWORD retValDoor = WaitForSingleObject(threadHandles[myIndex].intDoor, 0);
				if (retValDoor != WAIT_OBJECT_0) {
					S2CPlayerPacket intoDoorPacket;
					intoDoorPacket.id = myIndex;
					intoDoorPacket.type = S2CIntoDoor;
					SetEvent(threadHandles[myIndex].intDoor);
					for (int j = 0; j < 3; j++)
						send(threadHandles[j].clientSocket, reinterpret_cast<char*>(&intoDoorPacket), sizeof(S2CPlayerPacket), 0);
				}
			}
		}
	}
}

bool Client::have_role()
{
	return role;
}

STAGE_TYPE Client::get_curr_stage()
{
	return curr_stage_type;
}