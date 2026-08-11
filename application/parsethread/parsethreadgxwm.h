// SPDX-FileCopyrightText: 2019 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PARSETHREADGXWM_H
#define PARSETHREADGXWM_H

#include "parsethreadbase.h"

/**
 * @brief The ParseThreadGxwm class  GXWM(gxde-wlcom)日志获取线程
 */
class ParseThreadGxwm :  public ParseThreadBase
{
    Q_OBJECT
public:
    explicit ParseThreadGxwm(QObject *parent = nullptr);
    ~ParseThreadGxwm() override;

protected:
    void run() override;

    void handleGxwm();
};

#endif  // PARSETHREADGXWM_H
