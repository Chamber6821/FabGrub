//
// Created by Ivan on 26.04.2023.
//

#pragma once

#include "ScalarBase.h"

template <class T>
struct Min : ScalarBase<T> {
    using ScalarBase<T>::ScalarBase;
};

template <class T>
Min(T) -> Min<T>;
