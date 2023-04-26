//
// Created by Ivan on 26.04.2023.
//

#pragma once

#include "ScalarBase.h"

template <class T>
struct Max : ScalarBase<T> {
    using ScalarBase<T>::ScalarBase;
};

template <class T>
Max(T) -> Max<T>;
