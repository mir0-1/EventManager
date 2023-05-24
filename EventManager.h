#pragma once
#include <glib.h>
#include <NetworkManager.h>

class EventManager
{
	private:
		GMainContext* gMainContext;
		GMainLoop* gMainLoop;
		GThread* gLoopThread;
		GMutex gMutex;
		GCond gCond;
		bool lastAsyncState;
		void *lastEventData;
		
		static gpointer gLoopThreadFunc(gpointer thisObjData);
		
	public:
		void waitForAsync();
		void signalAsyncReady();
		void* getEventData();
		void setEventData(void* newData);
		gulong registerConnection(NMActiveConnection* connection, GCallback callback);
		void unregisterConnection(NMActiveConnection* connection, gulong* id);
	
		EventManager();
		~EventManager();
};
