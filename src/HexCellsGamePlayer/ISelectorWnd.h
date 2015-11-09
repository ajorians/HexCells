#pragma once

class ISelectorWnd
{
public:
   virtual ~ISelectorWnd() {}

   virtual void WindowMoved() = 0;
};
