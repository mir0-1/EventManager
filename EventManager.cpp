#include "EventManager.h"

EventManager::EventManager()
{
	g_mutex_init(&gMutex);
	g_cond_init(&gCond);
	gMainContext = g_main_context_get_thread_default();
	gLoopThread = g_thread_new(NULL, gLoopThreadFunc, gMainContext);
}

EventManager::~EventManager()
{
	g_main_loop_quit(gMainLoop);
	g_thread_join(gLoopThread);
	g_main_loop_unref(gMainLoop);
	g_mutex_clear(&gMutex);
	g_cond_clear(&gCond);
}

gpointer EventManager::gLoopThreadFunc(gpointer thisObjData)
{
	EventManager* thisObj = (EventManager*)thisObjData;
	thisObj->gMainLoop = g_main_loop_new(thisObj->gMainContext, false);
	g_main_loop_run(thisObj->gMainLoop);
	return NULL;
}

void EventManager::waitForAsync()
{
	g_mutex_lock(&gMutex);
	lastAsyncState = false;
	while (!lastAsyncState)
		g_cond_wait(&gCond, &gMutex);
	g_mutex_unlock(&gMutex);
}

void EventManager::signalAsyncReady()
{
	g_mutex_lock(&gMutex);
	lastAsyncState = true;
	g_cond_signal(&gCond);
	g_mutex_unlock(&gMutex);
}

void* EventManager::getEventData()
{
	return lastEventData;
}

void EventManager::setEventData(void* newData)
{
	lastEventData = newData;
}

guint EventManager::registerConnection(NMActiveConnection* connection, GCallback callback)
{
	return g_signal_connect(connection, "notify::" NM_ACTIVE_CONNECTION_STATE, callback, this);
}

void EventManager::unregisterConnection(NMActiveConnection* connection, guint* id)
{
	g_clear_signal_handler(id, connection);
}
