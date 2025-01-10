import React, { useEffect, useState } from 'react';
import { useWebSocket } from '../hooks/useWebSocket';

interface PrinterStatus {
  id: string;
  temperature: number;
  paperStatus: boolean;
  progress: number;
}

export const PrinterDashboard: React.FC = () => {
  const [printers, setPrinters] = useState<PrinterStatus[]>([]);
  const ws = useWebSocket('ws://localhost:8000/ws/printer');

  useEffect(() => {
    if (ws) {
      ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        setPrinters(current => 
          current.map(printer => 
            printer.id === data.id ? { ...printer, ...data } : printer
          )
        );
      };
    }
  }, [ws]);

  return (
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4 p-4">
      {printers.map(printer => (
        <div key={printer.id} className="bg-white rounded-lg shadow p-4">
          <h3 className="text-lg font-semibold">Printer {printer.id}</h3>
          <div className="mt-2">
            <p>Temperature: {printer.temperature}°C</p>
            <p>Paper Status: {printer.paperStatus ? 'Loaded' : 'Empty'}</p>
            <div className="w-full bg-gray-200 rounded-full h-2.5">
              <div 
                className="bg-blue-600 h-2.5 rounded-full" 
                style={{ width: `${printer.progress}%` }}
              ></div>
            </div>
          </div>
        </div>
      ))}
    </div>
  );
}; 