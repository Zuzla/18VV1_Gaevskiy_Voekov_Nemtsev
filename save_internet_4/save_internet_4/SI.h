#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string.h>

using namespace std;

//Прототипы функций
void c(const wchar_t * fileName);//проверяем
void r(const wchar_t * imageName, const wchar_t * textName);//вынимаем
void e(const wchar_t * imageName, const wchar_t * textName);//заносим

//Константы
const char cMask = 0x01;//Для выдёргивания из текста бита
const char cMask2 = 0xFE;//Для обнуления младшего бита байта изображения
const char cTag[5] = "SI";//метка

