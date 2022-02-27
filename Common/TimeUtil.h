#pragma once

// Seconds.
double time_now_d();

// Sleep. Does not necessarily have millisecond granularity, especially on Windows.
void sleep_ms(int ms);

void GetTimeFormatted(char formattedTime[13]);

// Range: 0-11
int GetCurrentMonth();