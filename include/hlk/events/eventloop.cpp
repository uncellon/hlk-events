/******************************************************************************
 * 
 * Copyright (C) 2021 Dmitry Plastinin
 * Contact: uncellon@yandex.ru, uncellon@gmail.com, uncellon@mail.ru
 * 
 * This file is part of the Hlk Events library.
 * 
 * Hlk Events is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as pubblished by the
 * Free Software Foundation, either version 3 of the License, or (at your 
 * option) any later version.
 * 
 * Hlk Events is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more
 * details
 * 
 * You should have received a copy of the GNU Lesset General Public License
 * along with Hlk Events. If not, see <https://www.gnu.org/licenses/>.
 * 
 *****************************************************************************/

#include "eventloop.h"

namespace Hlk {

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

EventLoop::EventLoop() {
    m_running = true;
    m_threadLoop = new std::thread(&EventLoop::loop, this);
}

EventLoop::~EventLoop() {
    m_running = false;
    m_cv.notify_one();
    m_threadLoop->join();
}

/******************************************************************************
 * Methods
 *****************************************************************************/

void EventLoop::pushTask(AbstractTask* task) {
    m_mutex.lock();
    m_tasks.emplace(task);
    m_mutex.unlock();
    m_cv.notify_one();
}

/******************************************************************************
 * Methods (Protected)
 *****************************************************************************/

void EventLoop::loop() {
    std::mutex cvMutex;
    std::unique_lock lock(cvMutex, std::defer_lock);

    while (m_running) {
        if (m_tasks.empty()) {
            m_cv.wait(lock);
            continue;
        }

        m_mutex.lock();
        auto task = m_tasks.front();
        m_tasks.pop();
        m_mutex.unlock();

        task->execute();
        delete task;
    }
}

} // namespace Hlk