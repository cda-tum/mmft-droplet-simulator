import dropletLib from './droplet';
import { SimulationResult } from './Interfaces';

dropletLib.onRuntimeInitialized = function () {
    console.log("--- API ---");
    const sim = new dropletLib.Simulator();

    console.log("--- flowRatePump ---");
    // flowRate pump
    const flowRate = 3e-11;
    let pump = sim.addFlowRatePump(-1, 0, flowRate);

    console.log("--- channels ---");
    // channels
    const cWidth = 100e-6;
    const cHeight = 30e-6;
    const cLength = 1000e-6;

    const c1 = sim.addChannel(0, 1, cHeight, cWidth, cLength);
    const c2 = sim.addChannel(1, 2, cHeight, cWidth, cLength);
    const c3 = sim.addChannel(2, 3, cHeight, cWidth, cLength);
    const c4 = sim.addChannel(2, 4, cHeight, cWidth, cLength);
    const c5 = sim.addChannel(3, 4, cHeight, cWidth, cLength);
    const c6 = sim.addChannel(4, -1, cHeight, cWidth, cLength);
    console.log("--- sink ---");
    sim.addSink(-1);
    console.log("--- ground ---");
    sim.addGround(-1);

    // fluids
    console.log("--- fluids ---");
    const fluid0 = sim.addFluid(1e-3, 1e3, 0.0);
    const fluid1 = sim.addFluid(3e-3, 1e3, 0.0);
    console.log("--- continuousPhase ---");
    sim.setContinuousPhase(fluid0);

    // droplet
    console.log("--- droplet ---");
    const dropletVolume = 1.5 * cWidth * cWidth * cHeight;
    const droplet0 = sim.addDroplet(fluid1, dropletVolume, 0.0, c1, 0.5);

    // check if chip is valid
    console.log("--- validity check chip ---");
    console.log(sim.checkChipValidity());

    // simulate
    console.log("--- simulate ---");
    const results: SimulationResult = JSON.parse(sim.simulate().toJson(-1));

    console.log(results);
}