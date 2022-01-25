#pragma once

namespace Kernel {

    static inline bool are_interrupts_enabled();
    static inline unsigned long save_irqdisable();
    static inline void irqrestore(unsigned long flags);
    static inline void disable_interrupts();
    static inline void enable_interrupts();
    static inline void call_interrupt(int int_no);

}
