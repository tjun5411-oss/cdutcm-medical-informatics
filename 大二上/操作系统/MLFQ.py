import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import must  # 保证matplotlib库能够正常调用的必要本地库

class MLFQ:
    def __init__(self):
        self.queues = [
            {'time_slice': 2, 'queue': []},  # 最高优先级队列
            {'time_slice': 4, 'queue': []},  # 中等优先级队列
            {'time_slice': 8, 'queue': []}  # 最低优先级队列
        ]
        self.time = 0
        self.history = []

    def add_process(self, process_id, burst_time):  # 添加进程
        self.queues[0]['queue'].append({  # 先进入最高优先级队列
            'id': process_id,
            'burst_time': burst_time,
            'remaining_time': burst_time
        })

    def run(self):
        while any(len(q['queue']) > 0 for q in self.queues):
            for i, queue_info in enumerate(self.queues):
                if queue_info['queue']:
                    process = queue_info['queue'].pop(0)
                    time_slice = queue_info['time_slice']

                    exec_time = min(process['remaining_time'], time_slice)
                    process['remaining_time'] -= exec_time

                    self.history.append({
                        '时间': self.time,
                        '进程': process['id'],
                        '队列': i,
                        '持续时间': exec_time,
                        '结束时间': self.time + exec_time
                    })
                    self.time += exec_time

                    if process['remaining_time'] > 0:
                        if i < len(self.queues) - 1:
                            self.queues[i + 1]['queue'].append(process)
                    break

    def display_results(self):
        df = pd.DataFrame(self.history)
        print("多级队列反馈调度过程:")
        print("=" * 50)
        print(df.to_string(index=False))

        print("\n调度甘特图:")
        print("=" * 30)

        # 创建文本甘特图
        timeline = []
        for _, row in df.iterrows():
            start = row['时间']
            end = row['结束时间']
            process = row['进程']
            queue = row['队列']

            for t in range(int(start), int(end)):
                if t >= len(timeline):
                    timeline.extend([' '] * (t - len(timeline) + 1))
                timeline[t] = process

        print("时间: ", end="")
        for i in range(len(timeline)):
            print(f"{i:2d}", end=" ")
        print()

        print("进程: ", end="")
        for i in range(len(timeline)):
            print(f" {timeline[i]} ", end="")
        print()

        print("\n队列颜色说明:")
        print("队列0 (高优先级): P1, P2, P3, P4 首次执行")
        print("队列1 (中优先级): 降级后的进程")
        print("队列2 (低优先级): 再次降级的进程")

        print(f"\n总执行时间: {self.time}")
        print(f"进程总数: {len(set(df['进程']))}")

        # 绘制matplotlib甘特图
        self.plot_gantt_chart(df)

    def plot_gantt_chart(self, df):
        # 为不同队列定义颜色
        queue_colors = {
            0: '#FF6B6B',  # 红色 - 队列0
            1: '#4ECDC4',  # 青色 - 队列1
            2: '#FFD166'  # 黄色 - 队列2
        }

        # 获取所有唯一的进程ID
        processes = sorted(df['进程'].unique())
        process_positions = {process: i for i, process in enumerate(processes)}

        # 创建图形
        fig, ax = plt.subplots(figsize=(12, 6))

        # 绘制每个进程的执行段
        for _, row in df.iterrows():
            process = row['进程']
            queue = row['队列']
            start_time = row['时间']
            duration = row['持续时间']
            end_time = row['结束时间']

            # 确定y轴位置
            y_pos = process_positions[process]

            # 绘制条形
            ax.barh(
                y=y_pos,
                width=duration,
                left=start_time,
                height=0.6,
                color=queue_colors[queue],
                edgecolor='black'
            )

            # 在条形中央添加进程ID
            center_x = start_time + duration / 2
            ax.text(
                center_x, y_pos,
                process,
                ha='center', va='center',
                fontweight='bold',
                fontsize=10
            )

            # 在条形上方添加持续时间
            ax.text(
                center_x, y_pos + 0.35,
                f'{duration}',
                ha='center', va='bottom',
                fontsize=8
            )

        # 设置y轴
        ax.set_yticks(list(process_positions.values()))
        ax.set_yticklabels(list(process_positions.keys()))
        ax.set_ylabel('进程')

        # 设置x轴
        ax.set_xlabel('时间')
        ax.set_xlim(0, self.time)

        # 添加标题
        ax.set_title('多级队列反馈调度甘特图', fontsize=14, fontweight='bold')

        # 创建图例
        legend_patches = [
            mpatches.Patch(color=queue_colors[0], label='队列0 (高优先级)'),
            mpatches.Patch(color=queue_colors[1], label='队列1 (中优先级)'),
            mpatches.Patch(color=queue_colors[2], label='队列2 (低优先级)')
        ]
        ax.legend(handles=legend_patches, loc='upper left')

        # 添加网格线
        ax.grid(True, axis='x', linestyle='--', alpha=0.3)

        # 调整布局并显示
        plt.tight_layout()
        plt.show()


# 测试
if __name__ == "__main__":
    scheduler = MLFQ()

    # 添加进程
    processes = [('P1', 6), ('P2', 4), ('P3', 8), ('P4', 3)]
    for pid, burst in processes:
        scheduler.add_process(pid, burst)

    # 运行调度
    scheduler.run()

    # 显示结果
    scheduler.display_results()